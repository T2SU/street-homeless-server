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

#include "world/map_state.hpp"
#include "users/change_map_request.hpp"

namespace hl::master
{
    struct game_world_dimension
    {
        std::unordered_map<uint32_t, std::shared_ptr<map_state>> maps; // key: map_sn
        uint32_t flag;

        [[nodiscard]] size_t calc_population() const;
    };

    class game_world
    {
    private:
        using store_map_type = std::unordered_map<std::string, std::shared_ptr<map_state>>;

        mutable std::mutex _mutex;
        std::unordered_map<uint32_t, std::shared_ptr<map_state>> _maps; // key: map sn
        store_map_type _fields;    // key: map scene
        store_map_type _instances; // key: map scene, value: acceptable instance
        std::unordered_map<uint32_t, game_world_dimension> _maps_of_servers; // key: server_idx

        uint32_t _sn_counter;

    public:
        game_world();

        void change_map(const std::shared_ptr<change_map_request>& req);
        void on_after_creation(uint32_t server_idx, uint32_t map_sn, bool success);
        void remove_player(uint64_t pid);

        void add_server(uint32_t server_idx, uint32_t flag);
        void remove_server(uint32_t server_idx);

    private:
        std::shared_ptr<map_state> create_map(const std::shared_ptr<change_map_request>& req);

        void put_map(const std::shared_ptr<map_state>& map);
        void request_map_creation(const std::shared_ptr<map_state>& map);
        void remove_map(const std::shared_ptr<map_state>& map);

        uint32_t retrieve_server_for_map(map_type type) const;
    };
}

#endif //STREET_HOMELESS_SERVER_GAME_WORLD_HPP
