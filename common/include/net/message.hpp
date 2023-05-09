//
// Created by TSU on 2023-05-09.
//

#ifndef STREET_HOMELESS_SERVER_MESSAGES_HPP
#define STREET_HOMELESS_SERVER_MESSAGES_HPP

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
