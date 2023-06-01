//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_ENTER_WORLD_REQ_HPP
#define STREET_HOMELESS_SERVER_ENTER_WORLD_REQ_HPP

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/master_session.hpp"
#include "net/master_server.hpp"
#include <net/packet_handler.hpp>

namespace hl::master::handlers
{
    struct migrate_region_req : public hl::packet_handler<master_session>
    {
    public:
        void handle_packet(master_session &session, in_buffer &in_buf) override;
    };
}

#endif //STREET_HOMELESS_SERVER_ENTER_WORLD_REQ_HPP
