//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_CA22223A8F1E4657B5FC8A77AA83750A
#define STREET_HOMELESS_SERVER_CA22223A8F1E4657B5FC8A77AA83750A

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/game_session.hpp"
#include "net/game_server.hpp"
#include <net/packet_handler.hpp>

namespace hl::game::handlers
{
    struct migrate_req : public hl::packet_handler<game_session>
    {
    public:
        void handle_packet(game_session &session, in_buffer &in_buf) override;
    };
}

#endif //STREET_HOMELESS_SERVER_CA22223A8F1E4657B5FC8A77AA83750A
