//
// Created by TSU on 2023-05-11.
//
#include "std.hpp"
#include "net/master.hpp"
#include "net/master_handler.hpp"
#include "mhandlers/map_management_req.hpp"
#include "mhandlers/enter_game_res.hpp"

hl::game::master_handler::master_handler()
    : _handlers()
{
    _handlers[hl::InternalServerMessage_MapManagementReq] = std::make_unique<hl::game::mhandlers::map_management_req>();
    _handlers[hl::InternalServerMessage_EnterGameRes] = std::make_unique<hl::game::mhandlers::enter_game_res>();
}

void hl::game::master_handler::process(hl::game::master &session, in_buffer &in_buf)
{
    auto packet = static_cast<hl::InternalServerMessage>(in_buf.read<uint16_t>());

    if (packet >= InternalServerMessage_Max)
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }

    if (_handlers[packet])
        _handlers[packet]->handle_packet(session, in_buf);
}
