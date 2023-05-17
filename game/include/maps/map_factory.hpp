//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_CDC56417A1C6443EA384CBB3CE2D7EA2
#define STREET_HOMELESS_SERVER_CDC56417A1C6443EA384CBB3CE2D7EA2

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "map.hpp"

namespace hl::game
{
    class map_factory
    {
    private:
        std::mutex _mutex;
        std::unordered_map<uint32_t, std::unique_ptr<map>> _maps;

    public:
        map_factory();

        bool create_map(uint32_t map_sn, const std::string& scene, map_type type);
        void remove_map(uint32_t map_sn);
        map* get_map(uint32_t map);

    private:
        static void load_map(hl::game::map& map);
        static void add_portal_to_map(hl::game::map& map, std::shared_ptr<hl::game::portal> pt);
    };
}

#endif //STREET_HOMELESS_SERVER_CDC56417A1C6443EA384CBB3CE2D7EA2
