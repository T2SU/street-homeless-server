//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "net/master_session.hpp"
#include "net/master_server.hpp"
#include "users/user_record.hpp"
#include "users/abstract_request.hpp"
#include "world/hangout.hpp"

hl::master::abstract_request::abstract_request(socket_sn_t master_socket_sn, player_id_t pid)
    : _master_socket_sn(master_socket_sn), _pid(pid)
{
}

out_buffer hl::master::abstract_request::make_reply(hl::InternalServerMessage msg) const
{
    out_buffer b(msg);
    b.write(get_user()->get_player_socket_sn());
    return b;
}

std::shared_ptr<hl::master::master_session> hl::master::abstract_request::get_session() const
{
    std::shared_ptr<hl::master::master_session> ret;

    hl::singleton<hl::master::master_server>::get().try_get(_master_socket_sn, ret);
    return ret;
}

std::shared_ptr<hl::master::user_record> hl::master::abstract_request::get_user() const
{
    return hl::singleton<hl::master::hangout>::get().find_user(_pid);
}
