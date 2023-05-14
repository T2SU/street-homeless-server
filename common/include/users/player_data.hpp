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
#include "io/out_buffer.hpp"

namespace hl
{
    class player_data
    {
    private:
        pb::PlayerData _data;

        std::string _map;
        std::string _sp;

    public:
        player_data();
        ~player_data();

        void load(uint64_t pid, sqlpp::mysql::connection& conn);
        void encode(out_buffer& obuf) const;

        const std::string& get_map() const;
        const std::string& get_sp() const;
        void set_map(const std::string &map);
        void set_sp(const std::string &sp);
    };
}


#endif //STREET_HOMELESS_SERVER_3377B54D05C94A238B1D5BEEE9DF1348
