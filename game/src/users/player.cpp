//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "users/player.hpp"

hl::game::player::player()
    : _pid()
    , _uid()
    , _stat()
    , _money()
    , _inventory()
    , _name()
    , _map_sn()
{
}

void hl::game::player::init(pb::PlayerData &data, uint32_t map_sn)
{
    _pid = data.pid();
    _uid = data.uid();
    _name = data.name();
    _map_sn = map_sn;
    _stat.init(data);
    init_inventory(data);
}

void hl::game::player::init_inventory(pb::PlayerData &data)
{
    _money = data.inventory().money();
    for (const auto& element : data.inventory().items())
    {
        _inventory.emplace_back(element.item_id(), element.amount());
    }
}
