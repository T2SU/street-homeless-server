//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_acceptor.hpp"

login_acceptor::login_acceptor(uv_loop_t *loop, std::string bind_address, uint16_t bind_port)
        : acceptor(loop, bind_address, bind_port)
{
}

login_session *login_acceptor::create_session()
{
    return nullptr;
}

void login_acceptor::on_accept(login_session *session)
{
}
