//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_PACKET_HPP
#define STREET_HOMELESS_SERVER_PACKET_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl
{
    #pragma pack(push, 2)

    typedef struct packet_header
    {
        uint16_t checksum;
        uint16_t length;
    } packet_header;

    #pragma pack(pop)
}

#endif //STREET_HOMELESS_SERVER_PACKET_HPP
