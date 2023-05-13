//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/game_session.hpp"
#include "net/game_server.hpp"
#include "net/game_handler.hpp"

hl::game::game_session::game_session(server* server, uint32_t id, uint32_t socket_sn)
        : abstract_session(server, id, socket_sn)
{}

void hl::game::game_session::on_close(close_reason reason)
{
}

void hl::game::game_session::on_packet(in_buffer &in_buffer)
{
    hl::singleton<hl::game::game_handler>::get().process(*this, in_buffer);
}

hl::game::game_server &hl::game::game_session::get_server()
{
    return *reinterpret_cast<hl::game::game_server*>(_server);
}

const char *hl::game::game_session::get_type_name() const
{
    return "game_session";
}

const pb::AccountData &hl::game::game_session::get_account_data() const
{
    return _account_data;
}

void hl::game::game_session::set_account_data(pb::AccountData account_data)
{
    _account_data = std::move(account_data);
}
