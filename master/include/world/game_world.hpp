//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_GAME_WORLD_HPP
#define STREET_HOMELESS_SERVER_GAME_WORLD_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "world/region.hpp"
#include "users/migration_region_request.hpp"

namespace hl::master
{
    struct game_world_dimension
    {
        std::unordered_map<region_sn_t, std::shared_ptr<region>> regions;
        uint32_t flag;

        [[nodiscard]] size_t calc_population() const;
    };

    class game_world
    {
    private:
        using store_region_t = std::unordered_map<region_id_t, std::map<channel_id_t, std::shared_ptr<region>>>;

        mutable std::recursive_mutex _mutex;
        store_region_t _regions;    // key: region_id_t -> channel_id_t
        std::unordered_map<server_idx_t, game_world_dimension> _dimensions; // key: server_idx

        region_sn_t _sn_counter;

    public:
        game_world();

        void migrate_region(const std::shared_ptr<migration_region_request>& req);
        void on_after_creation(server_idx_t server_idx, region_sn_t region_sn, bool success);
        void remove_player(player_id_t pid);

        void add_server(server_idx_t server_idx, uint32_t flag);
        void remove_server(server_idx_t server_idx);

    private:
        void put_region_to_game_world(const std::shared_ptr<region>& region);
        void remove_region(const std::shared_ptr<region>& region);

        server_idx_t retrieve_server_for_region(region_type type) const;

        static void request_region_creation(const std::shared_ptr<region>& region);
    };
}

#endif //STREET_HOMELESS_SERVER_GAME_WORLD_HPP
