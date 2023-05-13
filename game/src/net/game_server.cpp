//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/game_server.hpp"

void hl::game::game_server::on_accept(hl::game::game_session *session)
{
    LOGI << "Accepted game session from [" << session->get_remote_endpoint() << "]";
}
