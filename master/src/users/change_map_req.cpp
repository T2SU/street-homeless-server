//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "net/master_session.hpp"
#include "users/change_map_req.hpp"

hl::master::change_map_req::change_map_req(uint32_t master_socket_sn, uint64_t pid, std::string scene, std::string sp, bool first_enter)
    : abstract_request(master_socket_sn, pid)
    , _scene(std::move(scene))
    , _sp(std::move(sp))
    , _first_enter(first_enter)
{
}
