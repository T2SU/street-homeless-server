//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_MAP_STATE_HPP
#define STREET_HOMELESS_SERVER_MAP_STATE_HPP

#include "users/change_map_request.hpp"

namespace hl::master
{
    class map_state
    {
    private:
        const uint32_t _map_sn;
        const uint32_t _server_idx;
        const map_type _map_type;
        const std::string _scene;
        map_load_state _state;
        std::queue<std::shared_ptr<change_map_request>> _queue;

        std::unordered_set<uint64_t> _players;

    public:
        map_state(uint32_t map_sn, uint32_t server_idx, map_type type, std::string scene);

        [[nodiscard]] inline auto get_server_idx() const { return _server_idx; }
        [[nodiscard]] inline auto get_map_sn() const { return _map_sn; }
        [[nodiscard]] inline auto get_map_type() const { return _map_type; }
        [[nodiscard]] inline auto get_scene() const { return _scene; }
        [[nodiscard]] inline bool empty() const { return _players.empty(); }
        [[nodiscard]] inline std::unordered_set<uint64_t>::size_type size() const { return _players.size(); }

        inline void set_state(map_load_state state) { _state = state; }
        [[nodiscard]] inline map_load_state get_state() const { return _state; }

        void add_player(std::shared_ptr<change_map_request> req);
        void remove_player(uint64_t pid);

        void add_queue(std::shared_ptr<change_map_request> req);
        void process_after_creation(bool success);
        void flush(const std::shared_ptr<change_map_request>& req, pb::ChangeMapResult success);
    };
}

#endif //STREET_HOMELESS_SERVER_MAP_STATE_HPP
