//
// Created by T2SU on 2023/05/16.
//


#include "std.hpp"
#include "handlers/move_player_req.hpp"
#include "users/player.hpp"
#include "maps/map.hpp"

void hl::game::handlers::move_player_req::handle_packet(game_session &session, in_buffer &in_buf)
{
    const auto player = session.get_player();
    player->get_map()->on_move_player(player, in_buf);
}
