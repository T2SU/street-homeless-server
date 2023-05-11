//
// Created by TSU on 2023-05-12.
//

#ifndef STREET_HOMELESS_SERVER_SERVER_HPP
#define STREET_HOMELESS_SERVER_SERVER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif


namespace hl
{
    class abstract_session;

    class server
    {
    public:
        virtual void remove_from_connected(abstract_session* session) = 0;
        virtual void release_session(abstract_session *session) = 0;
    };
}

#endif //STREET_HOMELESS_SERVER_SERVER_HPP
