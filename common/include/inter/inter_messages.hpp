//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_INTER_MESSAGES_HPP
#define STREET_HOMELESS_SERVER_INTER_MESSAGES_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl
{
    enum InternalServerMessage : uint16_t
    {
        InternalServerMessage_SetUpRes,
        InternalServerMessage_OnMigrateRes,
        InternalServerMessage_MigrateRegionRes,
        InternalServerMessage_RegionManagementReq,

        InternalServerMessage_Max
    };

    enum InternalClientMessage : uint16_t
    {
        InternalClientMessage_SetUpReq,
        InternalClientMessage_OnMigrateReq,
        InternalClientMessage_MigrateRegionReq,
        InternalClientMessage_LogoutReq,
        InternalClientMessage_RegionManagementRes,
        InternalClientMessage_SignOutReq,

        InternalClientMessage_Max
    };
}

#endif //STREET_HOMELESS_SERVER_INTER_MESSAGES_HPP
