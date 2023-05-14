//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/master.hpp"
#include "net/master_handler.hpp"

hl::login::master::master(server* server, uint32_t id, uint32_t socket_sn)
        : abstract_session(server, id, socket_sn)
{}

void hl::login::master::on_close(close_reason reason)
{
    LOGF << "Lost connection with master. terminate process.";
    exit(EXIT_FAILURE);
}

void hl::login::master::on_packet(in_buffer &in_buffer)
{
    hl::singleton<hl::login::master_handler>::get().process(*this, in_buffer);
}

const char *hl::login::master::get_type_name() const
{
    return "master";
}

void hl::login::master::on_connect()
{
    out_buffer obuf(hl::InternalClientMessage_SetUpReq);
}
