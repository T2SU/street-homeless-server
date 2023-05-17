//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_F848CB75920D4091945CC22202BE9CC2
#define STREET_HOMELESS_SERVER_F848CB75920D4091945CC22202BE9CC2

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "portal.hpp"
#include "objects/field_object.hpp"

namespace hl::game
{
    class player;

    class map
    {
    private:
        const uint32_t _map_sn;
        const std::string _scene;
        const map_type _type;

        std::unordered_map<std::string, std::shared_ptr<portal>> _portals;
        std::vector<std::shared_ptr<portal>> _starting_points;

        mutable std::mutex _mutex;
        std::unordered_map<uint64_t, std::shared_ptr<player>> _players;
        std::unordered_map<uint64_t, std::shared_ptr<field_object>> _objects;

        friend class map_factory;

    public:
        map(uint32_t map_sn, std::string scene, map_type type);

        uint32_t get_map_sn() const;
        const std::string &get_scene() const;
        map_type get_type() const;

        void put_on_portal(const std::shared_ptr<player>& player, std::string& sp);
        void add_player(const std::shared_ptr<player>& player);
        void remove_player(uint64_t pid);
        void on_move_player(const std::shared_ptr<player>& player, in_buffer& in);

    private:
        std::shared_ptr<hl::game::portal> get_portal(const std::string& pt) const;
        std::shared_ptr<player> find_player(uint64_t pid);

        std::shared_ptr<field_object> find_object_no_lock(uint64_t object_id) const;

        inline auto get_objects() const
        {
            return _objects | std::views::transform([](const auto& pair){ return pair.second; });
        }

        inline auto get_players() const
        {
            return _players | std::views::transform([](const auto& pair){ return pair.second; });
        }

        void broadcast(const out_buffer& out, uint64_t exception_pid = 0);
    };
}

#endif //STREET_HOMELESS_SERVER_F848CB75920D4091945CC22202BE9CC2
