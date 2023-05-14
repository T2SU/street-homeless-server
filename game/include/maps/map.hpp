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

namespace hl::game
{
    class player;

    class map
    {
    private:
        const uint32_t _map_sn;
        const std::string _scene;
        const map_type _type;

        mutable std::mutex _mutex;
        std::unordered_map<uint64_t, std::shared_ptr<player>> _players;

    public:
        map(uint32_t map_sn, std::string scene, map_type type);

        uint32_t get_map_sn() const;
        const std::string &get_scene() const;
        map_type get_type() const;

        void add_player(const std::shared_ptr<player>& player, const std::string& sp);
        void remove_player(uint64_t pid);
        std::shared_ptr<player> find_player(uint64_t pid);
    };
}

#endif //STREET_HOMELESS_SERVER_F848CB75920D4091945CC22202BE9CC2
