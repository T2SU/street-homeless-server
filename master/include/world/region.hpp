//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_MAP_STATE_HPP
#define STREET_HOMELESS_SERVER_MAP_STATE_HPP

#include "users/migration_region_request.hpp"

namespace hl::master
{
    class region : public std::enable_shared_from_this<region>
    {
    private:
        const region_id_t _region_id;
        const region_sn_t _region_sn;
        const region_type _region_type;
        const size_t _max_players;
        const channel_id_t _channel_id;
        const server_idx_t _server_idx;
        region_load_state _state;
        std::queue<std::shared_ptr<migration_region_request>> _queue;

        std::unordered_set<player_id_t> _players;

        region(region_sn_t region_sn, channel_id_t channel_id, server_idx_t server_idx, const map_data& map_data, const region_info& region_info);
    public:

        [[nodiscard]] inline auto get_server_idx() const { return _server_idx; }
        [[nodiscard]] inline auto get_region_sn() const { return _region_sn; }
        [[nodiscard]] inline auto get_channel_id() const { return _channel_id; }
        [[nodiscard]] inline auto get_region_id() const { return _region_id; }
        [[nodiscard]] inline auto get_region_type() const { return _region_type; }
        [[nodiscard]] inline auto get_max_players() const { return _max_players; }
        [[nodiscard]] inline bool empty() const { return _players.empty(); }
        [[nodiscard]] inline decltype(_players)::size_type size() const { return _players.size(); }
        [[nodiscard]] inline bool is_crowded() const { return size() * 100 / get_max_players() > 70; }
        [[nodiscard]] inline bool is_full() const { return size() >= get_max_players(); }
        [[nodiscard]] inline bool is_saturated() const { return size() * 100 /  get_max_players() >= 120; }

        inline void set_state(region_load_state state) { _state = state; }
        [[nodiscard]] inline region_load_state get_state() const { return _state; }

        void add_player(std::shared_ptr<migration_region_request> req);
        void remove_player(player_id_t pid);

        void add_queue(std::shared_ptr<migration_region_request> req);
        void process_after_creation(bool success);
        void flush(const std::shared_ptr<migration_region_request>& req, pb::ChangeMapResult result);

        [[nodiscard]]
        inline static auto create(region_sn_t region_sn, channel_id_t channel_id, server_idx_t server_idx, const map_data& map_data, const region_info& region_info)
        {
            return std::shared_ptr<hl::master::region>(new region(region_sn, channel_id, server_idx, map_data, region_info));
        }
    };
}

#endif //STREET_HOMELESS_SERVER_MAP_STATE_HPP
