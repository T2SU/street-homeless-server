//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_LOGIN_ACCEPTOR_HPP
#define STREET_HOMELESS_SERVER_LOGIN_ACCEPTOR_HPP

#include "login_session.hpp"
#include <net/acceptor.hpp>

class login_acceptor : public acceptor<login_session>
{
public:
    login_acceptor(uv_loop_t *loop, std::string bind_address, uint16_t bind_port);

protected:
    login_session* create_session() override;
    void on_accept(login_session* session) override;
};


#endif //STREET_HOMELESS_SERVER_LOGIN_ACCEPTOR_HPP
