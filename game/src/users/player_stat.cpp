//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "users/player_stat.hpp"

hl::game::player_stat::player_stat()
    : _stat()
    , _appearance()
{
}

void hl::game::player_stat::init(pb::PlayerData &data)
{
    _stat = data.stat();
    _appearance = data.appearance();
}

const pb::PlayerAppearance &hl::game::player_stat::get_appearance() const
{
    return _appearance;
}
