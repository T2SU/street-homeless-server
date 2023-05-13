//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_LOGIN_HANDLER_HPP
#define STREET_HOMELESS_SERVER_HANDLER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::master
{
    class login_handler
    {
    private:
        std::array<std::unique_ptr<hl::packet_handler<master_session>>, hl::InternalClientMessage_Max> _handlers;

    public:
        login_handler();

        void process(master_session& session, in_buffer& in_buf);
    };
}

#endif //STREET_HOMELESS_SERVER_LOGIN_HANDLER_HPP
