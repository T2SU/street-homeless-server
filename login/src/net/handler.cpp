//
// Created by TSU on 2023-05-11.
//
#include "std.hpp"
#include "net/login_session.hpp"
#include "net/handler.hpp"
#include "handlers/check_alive_res.hpp"
#include "handlers/login_req.hpp"
#include "handlers/select_character_req.hpp"

hl::login::handler::handler()
    : _handlers()
{
    _handlers[pb::ClientMessage::CheckAliveRes] = std::make_unique<hl::login::handlers::check_alive_res>();
    _handlers[pb::ClientMessage::LoginReq] = std::make_unique<hl::login::handlers::login_req>();
    _handlers[pb::ClientMessage::SelectCharacterReq] = std::make_unique<hl::login::handlers::select_character_req>();
}

void hl::login::handler::process(hl::login::login_session &session, in_buffer &in_buf)
{
    auto packet = static_cast<pb::ClientMessage>(in_buf.read<uint16_t>());

    if (!pb::ClientMessage_IsValid(packet))
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }

    _handlers[packet]->handle_packet(session, in_buf);
}
