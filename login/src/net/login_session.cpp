//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_session.hpp"

login_session::login_session(uint32_t id)
        : abstract_session(id)
{}

void login_session::on_packet(const in_buffer &in_buffer)
{

}

void login_session::on_close(close_reason reason)
{

}
