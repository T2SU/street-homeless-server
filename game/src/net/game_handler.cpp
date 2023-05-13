//
// Created by TSU on 2023-05-11.
//
#include "std.hpp"
#include "net/game_session.hpp"
#include "net/game_handler.hpp"
#include "handlers/check_alive_res.hpp"

hl::game::game_handler::game_handler()
    : _handlers()
{
    _handlers[pb::ClientMessage_CheckAliveRes] = std::make_unique<hl::game::handlers::check_alive_res>();
}

void hl::game::game_handler::process(hl::game::game_session &session, in_buffer &in_buf)
{
    auto packet = static_cast<pb::ClientMessage>(in_buf.read<uint16_t>());

    if (!pb::ClientMessage_IsValid(packet))
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }

    if (_handlers[packet])
        _handlers[packet]->handle_packet(session, in_buf);
}
