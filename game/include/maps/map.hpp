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
#include "../objects/field_object.hpp"

namespace hl::game
{
    class player;

    class map
    {
    private:
        const std::string _scene;
        const region_id_t _region_id;
        const region_sn_t _region_sn;

        std::unordered_map<std::string, std::shared_ptr<portal>> _portals;
        std::vector<std::shared_ptr<portal>> _starting_points;

        mutable std::mutex _mutex;
        std::unordered_map<player_id_t, std::shared_ptr<player>> _players;
        std::unordered_map<uint64_t, std::shared_ptr<field_object>> _objects;

        friend class map_factory;

    public:
        map(std::string scene, region_id_t id, region_sn_t sn);

        [[nodiscard]] const std::string &get_scene() const;
        [[nodiscard]] region_id_t get_region_id() const;
        [[nodiscard]] region_sn_t get_region_sn() const;

        void put_on_portal(const std::shared_ptr<player>& player, std::string& sp);
        void add_player(const std::shared_ptr<player>& player);
        void remove_player(player_id_t pid);
        void on_move_player(const std::shared_ptr<player>& player, in_buffer& in);
        void on_change_map_req(const std::shared_ptr<player>& player, in_buffer& in);

    private:
        std::shared_ptr<hl::game::portal> get_portal(const std::string& pt) const;
        std::shared_ptr<player> find_player(player_id_t pid);

        std::shared_ptr<field_object> find_object_no_lock(uint64_t object_id) const;

        inline auto get_objects() const
        {
            return _objects | std::views::values;
        }

        inline auto get_players() const
        {
            return _players | std::views::values;
        }

        void broadcast(const out_buffer& out, player_id_t exception_pid = 0);

        void change_map_local(const std::shared_ptr<player>& player, const std::shared_ptr<portal>& pt) const;
        static void request_migrate_region(const std::shared_ptr<player>& player, const std::shared_ptr<portal>& pt, const std::string& device_id);
    };
}

#endif //STREET_HOMELESS_SERVER_F848CB75920D4091945CC22202BE9CC2
