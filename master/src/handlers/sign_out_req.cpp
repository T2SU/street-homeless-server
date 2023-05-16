//
// Created by T2SU on 2023/05/14.
//


#include "std.hpp"
#include "handlers/sign_out_req.hpp"
#include "world/game_world.hpp"
#include "world/hangout.hpp"

void hl::master::handlers::sign_out_req::handle_packet(master_session &session, in_buffer &in_buf)
{
    auto pid = in_buf.read<uint64_t>();

    GAME_WORLD.remove_player(pid);
    HANGOUT.remove_user(pid);

    LOGD << "Signed out player " << pid;
}
