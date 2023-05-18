//
// Created by TSU on 2023-05-18.
//


#include "std.hpp"
#include "handlers/change_map_req.hpp"
#include "users/player.hpp"
#include "maps/map.hpp"

void hl::game::handlers::change_map_req::handle_packet(game_session &session, in_buffer &in_buf)
{
    const auto player = session.get_player();
    if (!player) return;
    auto map = player->get_map();
    if (!map) return;
    map->on_change_map_req(player, in_buf);
}
