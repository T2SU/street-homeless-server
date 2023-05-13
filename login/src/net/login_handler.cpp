//
// Created by TSU on 2023-05-11.
//
#include "std.hpp"
#include "net/login_session.hpp"
#include "net/login_handler.hpp"
#include "handlers/check_alive_res.hpp"
#include "handlers/login_req.hpp"
#include "handlers/select_character_req.hpp"
#include "handlers/create_character_req.hpp"
#include "handlers/delete_character_req.hpp"
#include "handlers/character_list_req.hpp"

hl::login::login_handler::login_handler()
    : _handlers()
{
    _handlers[pb::ClientMessage_CheckAliveRes] = std::make_unique<hl::login::handlers::check_alive_res>();
    _handlers[pb::ClientMessage_LoginReq] = std::make_unique<hl::login::handlers::login_req>();
    _handlers[pb::ClientMessage_CharacterListReq] = std::make_unique<hl::login::handlers::character_list_req>();
    _handlers[pb::ClientMessage_CreateCharacterReq] = std::make_unique<hl::login::handlers::create_character_req>();
    _handlers[pb::ClientMessage_DeleteCharacterReq] = std::make_unique<hl::login::handlers::delete_character_req>();
    _handlers[pb::ClientMessage_SelectCharacterReq] = std::make_unique<hl::login::handlers::select_character_req>();
}

void hl::login::login_handler::process(hl::login::login_session &session, in_buffer &in_buf)
{
    auto packet = static_cast<pb::ClientMessage>(in_buf.read<uint16_t>());

    if (!pb::ClientMessage_IsValid(packet))
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }

    if (_handlers[packet])
        _handlers[packet]->handle_packet(session, in_buf);
}
