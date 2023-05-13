//
// Created by TSU on 2023-05-13.
//
#include "std.hpp"
#include "net/master_server.hpp"

void hl::master::master_server::on_accept(hl::master::master_session *session)
{
    LOGI << "Accepted master session from [" << session->get_remote_endpoint() << "]";
}

hl::database::accessor &hl::master::master_server::accessor()
{
    return *_accessor[(_accessor_robin++) % _accessor_count];
}
