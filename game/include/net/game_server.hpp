//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_GAME_SERVER_HPP
#define STREET_HOMELESS_SERVER_GAME_SERVER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "game_session.hpp"

namespace hl::game
{
    class game_server : public abstract_server<game_session>
    {
    private:

    public:
        game_server() : abstract_server()
        {
        }

        void on_accept(game_session* session) override;
    };
}

#endif //STREET_HOMELESS_SERVER_GAME_SERVER_HPP
