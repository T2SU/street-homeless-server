//
// Created by TSU on 2023-05-14.
//


#include "std.hpp"
#include "handlers/enter_game_req.hpp"
#include "users/user_record.hpp"
#include "world/hangout.hpp"

static void send_error(hl::master::master_session &session, uint32_t socket_sn)
{
    out_buffer obuf(hl::InternalServerMessage_EnterGameRes);
    obuf.write(socket_sn);
    obuf.write(false);
    session.write(obuf);
}

void hl::master::handlers::enter_game_req::handle_packet(master_session &session, in_buffer &in_buf)
{
    const auto socket_sn = in_buf.read<uint32_t>();
    const auto device_id = in_buf.read_str();
    const auto remote_address = in_buf.read_str();
    const auto pid = in_buf.read<uint64_t>();

    auto user = HANGOUT.find_user(pid);
    if (!user)
    {
        LOGW << "user " << pid << " is not on hangout.";
        send_error(session, socket_sn);
        return;
    }
    if (user->get_state() != user_state::migrating)
    {
        LOGW << "user " << pid << " is not migrating state";
        send_error(session, socket_sn);
        return;
    }
    // 데이터 환경에서, TCP연결마다 아이피가 변한다..?
//    if (user->get_remote_address() != remote_address)
//    {
//        LOGW << "user " << pid << " ip address error.  server:[" << user->get_remote_address() << "], now:[" << remote_address << "]";
//        send_error(session, socket_sn);
//        return;
//    }
    if (user->get_device_id() != device_id)
    {
        LOGW << "user " << pid << " device id error.  server:[" << user->get_device_id() << "], now:[" << device_id << "]";
        send_error(session, socket_sn);
        return;
    }
    user->set_player_socket_sn(socket_sn);
    user->set_server_idx(session.get_idx());
    user->set_state(user_state::connected);

    out_buffer obuf(hl::InternalServerMessage_EnterGameRes);
    obuf.write(socket_sn);
    obuf.write(true);
    obuf.write(pid);
    obuf.write(user->get_map_sn());
    
    const auto& data = user->get_player_data();
    obuf.write_str(data.get_map());
    obuf.write_str(data.get_sp());
    obuf.write<uint8_t>(user->inc_migration_count() == 0 ? 1 : 0);
    data.encode(obuf);

    session.write(obuf);
}
