//
// Created by TSU on 2023-05-14.
//


#include "std.hpp"
#include "handlers/enter_game_world_req.hpp"
#include "net/master.hpp"

void hl::game::handlers::enter_game_world_req::handle_packet(game_session &session, in_buffer &in_buf)
{
    const auto device_id = in_buf.read_str();
    const auto pid = in_buf.read<uint64_t>();

    if (session.get_player())
    {
        LOGW << "Already have player. session will be closed force.";
        session.close();
        return;
    }

    out_buffer obuf(hl::InternalClientMessage_EnterGameReq);
    obuf.write(session.get_socket_sn());
    obuf.write_str(device_id);
    obuf.write_str(session.get_remote_address());
    obuf.write(pid);
    MASTER->write(obuf);
}
