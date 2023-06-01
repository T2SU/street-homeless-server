//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_AADB7FE6B7804ABDBD4C1685F97B77D2
#define STREET_HOMELESS_SERVER_AADB7FE6B7804ABDBD4C1685F97B77D2

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "users/user_record.hpp"

namespace hl::master
{
    class hangout
    {
    private:
        mutable std::mutex _mutex;
        std::unordered_map<player_id_t, std::shared_ptr<user_record>> _users;

    public:
        hangout();

        std::shared_ptr<user_record> find_user(player_id_t pid) const;
        void add_user(std::shared_ptr<user_record> user);
        void remove_user(player_id_t pid);
    };
}

#endif //STREET_HOMELESS_SERVER_AADB7FE6B7804ABDBD4C1685F97B77D2
