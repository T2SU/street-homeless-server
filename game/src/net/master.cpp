//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/master.hpp"
#include "net/master_handler.hpp"
#include "net/game_server.hpp"

hl::game::master::master(server* server, uint32_t id, uint32_t socket_sn)
        : abstract_session(server, id, socket_sn)
{}

void hl::game::master::on_close(close_reason reason)
{
    LOGF << "Lost connection with master. terminate process.";
    exit(EXIT_FAILURE);
}

void hl::game::master::on_packet(in_buffer &in_buffer)
{
    hl::singleton<hl::game::master_handler>::get().process(*this, in_buffer);
}

const char *hl::game::master::get_type_name() const
{
    return "master";
}

void hl::game::master::on_connect()
{
    out_buffer obuf(hl::InternalClientMessage_SetUpReq);
    hl::singleton<hl::game::game_server>::get().encode_config(obuf);
    write(obuf);
}
