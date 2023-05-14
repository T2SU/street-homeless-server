//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_E004541560BA448BB32EC3266A7EA785
#define STREET_HOMELESS_SERVER_E004541560BA448BB32EC3266A7EA785

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "player_stat.hpp"
#include "items/item.hpp"

namespace hl::game
{
    class player
    {
    private:
        player_stat _stat;
        std::vector<item> _inventory;
    };
}

#endif //STREET_HOMELESS_SERVER_E004541560BA448BB32EC3266A7EA785
