//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "net/master_session.hpp"
#include "users/migration_region_request.hpp"

hl::master::migration_region_request::migration_region_request(socket_sn_t master_socket_sn, player_id_t pid, std::string scene, std::string sp, bool first_enter)
    : abstract_request(master_socket_sn, pid)
    , _scene(std::move(scene))
    , _sp(std::move(sp))
    , _first_enter(first_enter)
{
}
