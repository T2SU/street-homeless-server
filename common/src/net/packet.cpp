//
// Created by TSU on 2023-05-07.
//
#include "std_common.hpp"
#include "net/packet.hpp"

packet::packet(const packet_header& hd)
        : header(hd)
        , content(hd.length) {}
