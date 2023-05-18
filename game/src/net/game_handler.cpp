//
// Created by TSU on 2023-05-11.
//
#include "std.hpp"
#include "net/game_session.hpp"
#include "net/game_handler.hpp"
#include "handlers/check_alive_res.hpp"
#include "handlers/enter_game_world_req.hpp"
#include "handlers/move_player_req.hpp"
#include "handlers/change_map_req.hpp"

hl::game::game_handler::game_handler()
    : _handlers()
{
    _handlers[pb::ClientMessage_CheckAliveRes] = std::make_unique<hl::game::handlers::check_alive_res>();
    _handlers[pb::ClientMessage_EnterGameWorldReq] = std::make_unique<hl::game::handlers::enter_game_world_req>();
    _handlers[pb::ClientMessage_MovePlayerReq] = std::make_unique<hl::game::handlers::move_player_req>();
    _handlers[pb::ClientMessage_ChangeMapReq] = std::make_unique<hl::game::handlers::change_map_req>();
}

void hl::game::game_handler::process(hl::game::game_session &session, in_buffer &in_buf)
{
    auto packet = static_cast<pb::ClientMessage>(in_buf.read<uint16_t>());

    if (!pb::ClientMessage_IsValid(packet))
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }
    if (packet != pb::ClientMessage_EnterGameWorldReq && packet != pb::ClientMessage_CheckAliveRes)
    {
        if (session.is_migrating_to_another())
        {
            LOGV << "Ignored packet [" << packet << "] due to user is migrating.";
            return;
        }
    }

    if (_handlers[packet])
        _handlers[packet]->handle_packet(session, in_buf);
}
