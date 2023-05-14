//
// Created by T2SU on 2023/05/14.
//

#ifndef STREET_HOMELESS_SERVER_13A77390E3944400B1D8D8485ECA3144
#define STREET_HOMELESS_SERVER_13A77390E3944400B1D8D8485ECA3144

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/master.hpp"
#include "net/login_server.hpp"
#include <net/packet_handler.hpp>

namespace hl::login::mhandlers
{
    struct change_map_res : public hl::packet_handler<master>
    {
    public:
        void handle_packet(master &session, in_buffer &in_buf) override;
    };
}

#endif //STREET_HOMELESS_SERVER_13A77390E3944400B1D8D8485ECA3144
