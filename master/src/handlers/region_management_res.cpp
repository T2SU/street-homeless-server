//
// Created by T2SU on 2023/05/15.
//


#include "std.hpp"
#include "handlers/region_management_res.hpp"
#include "world/game_world.hpp"

void hl::master::handlers::region_management_res::handle_packet(master_session &session, in_buffer &in_buf)
{
    const auto map_sn = in_buf.read<uint32_t>();
    const auto success = in_buf.read<bool>();

    GAME_WORLD.on_after_creation(session.get_idx(), map_sn, success);
}
