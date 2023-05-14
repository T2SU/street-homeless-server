//
// Created by TSU on 2023-05-13.
//


#include <utility>

#include "std.hpp"
#include "handlers/change_map_req.hpp"
#include "users/user_record.hpp"
#include "world/game_world.hpp"
#include "world/hangout.hpp"
#include "users/change_map_request.hpp"

class first_load_player_job : public hl::database::job
{
private:
    const uint32_t _master_socket_sn;
    const uint32_t _socket_sn;
    const std::string _device_id;
    const std::string _remote_address;
    const uint64_t _pid;

private:
    void send_error(std::shared_ptr<hl::master::master_session> session, pb::ChangeMapResult error)
    {
        out_buffer out_buf(hl::InternalServerMessage_ChangeMapRes);
        out_buf.write(_socket_sn);
        out_buf.write(error);
        session->write(out_buf);
    }

public:
    first_load_player_job(uint32_t master_socket_sn, uint32_t socket_sn, std::string device_id, std::string remote_address,
                   uint64_t pid)
            : _master_socket_sn(master_socket_sn), _socket_sn(socket_sn), _device_id(std::move(device_id))
            , _remote_address(std::move(remote_address)), _pid(pid)
    {}

    void process(sqlpp::mysql::connection &conn) override
    {
        std::shared_ptr<hl::master::master_session> session;
        if (!hl::singleton<hl::master::master_server>::get().try_get(_master_socket_sn, session))
            return;
        if (HANGOUT.find_user(_pid))
        {
            send_error(session, pb::ChangeMapResult_AlreadyConnected);
            return;
        }

        hl::player_data data;

        try
        {
            data.load(_pid, conn);

            auto user = std::make_shared<hl::master::user_record>(0, _socket_sn, _pid, _device_id, _remote_address);
            auto req = std::make_shared<hl::master::change_map_request>(_master_socket_sn, _pid, data.get_map(),
                                                                        data.get_sp(), true);
            user->set_player_data(std::move(data));

            LOGV << "adding user to hangout (" << user->get_pid() << ")";
            HANGOUT.add_user(user);

            LOGV << "changing user map to " << req->get_scene() << "," << req->get_starting_point() << " ("
                 << user->get_pid() << ")";
            GAME_WORLD.change_map(req);
        }
        catch (const sqlpp::exception &ex)
        {
            LOGE << "failed to load player " << _pid << ": " << ex.what();
            send_error(session, pb::ChangeMapResult_DatabaseError);
        }
        catch (const std::exception &ex)
        {
            LOGE << "failed to load player " << _pid << ": " << ex.what();
            send_error(session, pb::ChangeMapResult_UnknownError);
        }
    }
};

void hl::master::handlers::change_map_req::handle_packet(master_session &session, in_buffer &in_buf)
{
    const auto socket_sn = in_buf.read<uint32_t>();
    const auto device_id = in_buf.read_str();
    const auto remote_address = in_buf.read_str();
    const auto pid = in_buf.read<uint64_t>();
    const auto scene = in_buf.read_str();
    const auto sp = in_buf.read_str();

    if (scene.empty())
    {
        hl::singleton<hl::master::master_server>::get().accessor().post<first_load_player_job>
                (session.get_socket_sn(), socket_sn, device_id, remote_address, pid);
    }
    else
    {
        // TODO 게임 서버에서 맵 이동 전, scene, sp 유효성 검사
        auto req = std::make_shared<hl::master::change_map_request>(session.get_socket_sn(), pid, scene, sp, false);
        auto user = req->get_user();
        user->set_server_idx(0);
        user->set_player_socket_sn(0);
        user->set_new_map(scene, sp);
        user->set_state(user_state::migrating);
        GAME_WORLD.change_map(req);
    }
}

// 넣어야 하나?
//        if (!req->get_user() || !req->get_session())
//        {
//            out_buffer obuf(hl::InternalServerMessage_ChangeMapRes);
//            obuf.write(socket_sn);
//            obuf.write(false);
//            session.write(obuf);
//            return;
//        }


// 1. load player (from db) / 캐릭터 last_connected 업데이트
// 2. (성공시) 서버 메모리에 user_record 올리기
// 3. 접속할 맵 큐에 올리기

// 4. 생성 후 해당 맵에 대기중인 플레이어 처리
// 4-1. 해당 플레이어가 접속중일 경우, 해당 유저가 접속중인 서버에 응답 전송.
// 4-2. 해당 유저가 접속중인 서버는 해당 유저에게 맵이 존재하는 게임 서버로 접속 안내
// 4-3. 해당 유저의 user_record의 state는 migrating으로 변경

// 5. 유저가 접속한 게임 서버는 device_id, remote_address와 함께 마스터에 접속 요청.
// 6. 마스터는 해당 유저의 접속 정보를 검증, player_data를 반환. 동시에 해당 유저의 user_record의 state는 connected로 변경.




// 경쟁 조건?
//  플레이어가 migrating중 연결이 끊길 경우
//     [[유저소켓]]        [[마스터소켓]]     [[마스터Worker1]]      [[마스터Worker2]]
//   [SelectCharReq]
//                        [ChangeMapReq]
//                                         [                  ]
//                                         [                  ]
//                                         [                  ]
//       [close]                           [                  ]
//                         [UserExitReq]   [  change_map_job  ]
//                                         [                  ]     [             ]
//                                         [                  ]     [user_exit_job]
//                                         [                  ]     [             ]
//                                         [                  ]
//
// 해결 방안:
//     한 명의 유저의 job은 여러 스레드에서 동시에 실행되지 않아야 한다.
//       user_record에 std::queue<std::shared_ptr<job>>을 만든다.
//       accessor에 올라가는 job은 해당 user_record를 찾아서 큐잉된 job을 모두 처리하는 job으로 한다.
//           해당 user_record의 db처리 상태를 저장.
//           db_job_state가 queued면 큐에만 올리고 아무 것도 하지 않음.
//           db_job_state가 waiting이면 큐에 올리고, 임의의 accessor에 처리 요청
//         warpper job이 끝나면 해당 유저의 db_job_state를 queued에서 waiting으로 다시 변경.



