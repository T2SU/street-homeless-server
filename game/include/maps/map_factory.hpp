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
        const region_id_t _id;
        const region_sn_t _sn;

        mutable std::mutex _mutex;
        std::unordered_map<std::string, std::unique_ptr<map>> _maps;

    public:
        map_factory(region_id_t id, region_sn_t sn);

        map* get_map(const std::string& scene);

        [[nodiscard]] region_id_t get_region_id() const;
        [[nodiscard]] region_sn_t get_region_sn() const;
        [[nodiscard]] bool empty() const;

    private:
        bool create_map(const std::string& scene);
        void remove_map(const std::string& scene);

        static void load_map(hl::game::map& map);
        static void add_portal_to_map(hl::game::map& map, std::shared_ptr<hl::game::portal> pt);
    };
}

#endif //STREET_HOMELESS_SERVER_CDC56417A1C6443EA384CBB3CE2D7EA2
