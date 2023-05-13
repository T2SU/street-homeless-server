//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "net/master_session.hpp"
#include "net/master_server.hpp"
#include "net/handler.hpp"

hl::master::master_session::master_session(server* server, uint32_t id, uint32_t socket_sn)
        : abstract_session(server, id, socket_sn)
        , _type()
        , _idx()
        , _game_flag()
        , _setup(false)
{}

void hl::master::master_session::on_close(close_reason reason)
{
}

void hl::master::master_session::on_packet(in_buffer &in_buffer)
{
    hl::singleton<hl::master::login_handler>::get().process(*this, in_buffer);
}

hl::master::master_server &hl::master::master_session::get_server()
{
    return *reinterpret_cast<hl::master::master_server*>(_server);
}

const char *hl::master::master_session::get_type_name() const
{
    return "master_session";
}

bool hl::master::master_session::is_setup() const
{
    return _setup;
}

void hl::master::master_session::set_server_type(server_type type, uint32_t idx, uint32_t game_flag)
{
    _type = type;
    _idx = idx;
    _game_flag = game_flag;
}
