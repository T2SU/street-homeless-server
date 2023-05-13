//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_NET_HPP
#define STREET_HOMELESS_SERVER_NET_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/connector.hpp"
#include "net/abstract_server.hpp"
#include "net/abstract_session.hpp"
#include "net/network_exceptions.hpp"
#include "net/packet.hpp"
#include "net/packet_handler.hpp"
#include "net/socket_job.hpp"

#endif //STREET_HOMELESS_SERVER_NET_HPP
