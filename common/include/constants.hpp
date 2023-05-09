//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_CONSTANTS_HPP
#define STREET_HOMELESS_SERVER_CONSTANTS_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

constexpr uint32_t SessionBufferSize = 65536;
constexpr uint32_t MaxSession = 500;
constexpr int32_t DefaultBacklog = 50;

enum class close_reason { server_close, connection_reset, gracefully_shutdown, exception, unknown };

#endif //STREET_HOMELESS_SERVER_CONSTANTS_HPP
