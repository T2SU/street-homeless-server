//
// Created by TSU on 2023-05-18.
//

#ifndef STREET_HOMELESS_SERVER_55A6A8D1533543288CBF6773459D6FFA
#define STREET_HOMELESS_SERVER_55A6A8D1533543288CBF6773459D6FFA

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
    struct change_map_req : public hl::packet_handler<game_session>
    {
    public:
        void handle_packet(game_session &session, in_buffer &in_buf) override;
    };
}

#endif //STREET_HOMELESS_SERVER_55A6A8D1533543288CBF6773459D6FFA
