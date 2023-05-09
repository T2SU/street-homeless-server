//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_server.hpp"

void hl::login::login_server::on_accept(hl::login::login_session *session)
{
    LOGI << "Accepted login session from [" << session->get_remote_endpoint() << "]";
}
