//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_PACKET_HPP
#define STREET_HOMELESS_SERVER_PACKET_HPP

; // clangd bug (https://github.com/clangd/clangd/issues/1167)
#pragma pack(push, 2)

typedef struct packet_header
{
    uint16_t checksum;
    uint16_t length;
} packet_header;

#pragma pack(pop)

#endif //STREET_HOMELESS_SERVER_PACKET_HPP
