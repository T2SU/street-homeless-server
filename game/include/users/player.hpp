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
        uint64_t _pid;
        uint64_t _uid;
        player_stat _stat;
        int32_t _money;
        std::vector<item> _inventory;
        std::string _name;
        uint32_t _map_sn;


    public:
        player();

        void init(pb::PlayerData& data, uint32_t map_sn);

    private:
        void init_inventory(pb::PlayerData& data);
    };
}

#endif //STREET_HOMELESS_SERVER_E004541560BA448BB32EC3266A7EA785
