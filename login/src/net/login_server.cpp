//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_server.hpp"

void hl::login::login_server::on_accept(hl::login::login_session *session)
{
    LOGI << "Accepted login session from [" << session->get_remote_endpoint() << "]";
}

hl::database::accessor &hl::login::login_server::accessor()
{
    return *_accessor[(_accessor_robin++) % _accessor_count];
}
