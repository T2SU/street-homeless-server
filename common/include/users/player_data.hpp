//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_3377B54D05C94A238B1D5BEEE9DF1348
#define STREET_HOMELESS_SERVER_3377B54D05C94A238B1D5BEEE9DF1348

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "protobuf.hpp"

namespace hl
{
    class player_data
    {
    private:
        pb::PlayerData _data;
        pb::PlayerAppearance _appearance;
        pb::PlayerStat _player_stat;
        pb::Inventory _inventory;

        std::string _map;
        std::string _sp;

    public:
        player_data();

        void load(uint64_t pid, sqlpp::mysql::connection& conn);

        const std::string& get_map();
        const std::string& get_sp();
    };
}


#endif //STREET_HOMELESS_SERVER_3377B54D05C94A238B1D5BEEE9DF1348
