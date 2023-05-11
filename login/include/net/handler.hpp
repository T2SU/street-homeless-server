//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_HANDLER_HPP
#define STREET_HOMELESS_SERVER_HANDLER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::login
{
    class handler
    {
    private:
        std::array<std::unique_ptr<hl::packet_handler<login_session>>, pb::ClientMessage_ARRAYSIZE> _handlers;

    public:
        handler();

        void process(login_session& session, in_buffer& in_buf);
    };
}

#endif //STREET_HOMELESS_SERVER_HANDLER_HPP
