//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP
#define STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP

#include "login_session.hpp"
#include <net/abstract_server.hpp>

namespace hl::login
{
    class login_server : public abstract_server<login_session>
    {
    public:
        login_server()
            : abstract_server() {}

        void on_accept(login_session* session) override;
    };
}

#endif //STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP
