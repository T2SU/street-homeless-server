//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_AD9673CD0AC84335A16B3B120F40A88D
#define STREET_HOMELESS_SERVER_AD9673CD0AC84335A16B3B120F40A88D

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/master.hpp"
#include "net/game_server.hpp"
#include <net/packet_handler.hpp>

namespace hl::game::mhandlers
{
    struct map_management_req : public hl::packet_handler<master>
    {
    public:
        void handle_packet(master &session, in_buffer &in_buf) override;
    };
}

#endif //STREET_HOMELESS_SERVER_AD9673CD0AC84335A16B3B120F40A88D
