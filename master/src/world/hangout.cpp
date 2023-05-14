//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "world/hangout.hpp"

hl::master::hangout::hangout()
        : _mutex(), _users()
{}

std::shared_ptr<hl::master::user_record> hl::master::hangout::find_user(uint64_t pid) const
{
    synchronized (_mutex)
    {
        auto user = _users.find(pid);
        if (user != _users.end())
            return user->second;
    }
    return {};
}

void hl::master::hangout::add_user(std::shared_ptr<user_record> user)
{
    synchronized (_mutex)
    {
        _users[user->get_pid()] = user;
    }
}
