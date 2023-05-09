//
// Created by TSU on 2023-05-09.
//

#ifndef STREET_HOMELESS_SERVER_MESSAGES_HPP
#define STREET_HOMELESS_SERVER_MESSAGES_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

enum svr_msg : uint16_t
{
    CheckAliveReq,
    LoginRes
};

enum cli_msg : uint16_t
{
    CheckAliveRes,
    LoginReq
};

#endif //STREET_HOMELESS_SERVER_MESSAGES_HPP
