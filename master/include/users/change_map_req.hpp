//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_CHANGE_MAP_REQ_HPP
#define STREET_HOMELESS_SERVER_CHANGE_MAP_REQ_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "abstract_request.hpp"

namespace hl::master
{
    class change_map_req : public abstract_request
    {
    private:
        const std::string _scene;
        const std::string _sp;
        const bool _first_enter;

    public:
        change_map_req(uint32_t master_socket_sn, uint64_t pid, std::string scene, std::string sp, bool first_enter);

        [[nodiscard]] inline const auto& get_scene() const { return _scene; }
        [[nodiscard]] inline const auto& get_starting_point() const { return _sp; }
        [[nodiscard]] inline bool is_first_enter() const { return _first_enter; }
    };
}

#endif //STREET_HOMELESS_SERVER_CHANGE_MAP_REQ_HPP
