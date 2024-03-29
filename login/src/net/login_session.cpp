//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_session.hpp"
#include "net/login_server.hpp"
#include "net/login_handler.hpp"

hl::login::login_session::login_session(server* server, uint32_t id, uint32_t socket_sn)
        : abstract_session(server, id, socket_sn)
{}

void hl::login::login_session::on_close(close_reason reason)
{
}

void hl::login::login_session::on_packet(in_buffer &in_buffer)
{
    hl::singleton<hl::login::login_handler>::get().process(*this, in_buffer);
}

hl::login::login_server &hl::login::login_session::get_server()
{
    return *reinterpret_cast<hl::login::login_server*>(_server);
}

const char *hl::login::login_session::get_type_name() const
{
    return "login_session";
}

const pb::AccountData &hl::login::login_session::get_account_data() const
{
    return _account_data;
}

void hl::login::login_session::set_account_data(pb::AccountData account_data)
{
    _account_data = std::move(account_data);
}
