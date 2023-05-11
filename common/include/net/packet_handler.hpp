//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_PACKET_HANDLER_HPP
#define STREET_HOMELESS_SERVER_PACKET_HANDLER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "io/in_buffer.hpp"

namespace hl
{
    template<typename SessTy>
    struct packet_handler
    {
        virtual ~packet_handler() = default;
        virtual void handle_packet(SessTy& session, in_buffer& in_buf) = 0;
    };
}


#endif //STREET_HOMELESS_SERVER_PACKET_HANDLER_HPP
