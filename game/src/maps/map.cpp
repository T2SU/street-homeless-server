//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "maps/map.hpp"
#include "users/player.hpp"

hl::game::map::map(uint32_t map_sn, std::string scene, map_type type)
        : _map_sn(map_sn), _scene(std::move(scene)), _type(type)
        , _mutex(), _players()
{}

uint32_t hl::game::map::get_map_sn() const
{
    return _map_sn;
}

const std::string &hl::game::map::get_scene() const
{
    return _scene;
}

map_type hl::game::map::get_type() const
{
    return _type;
}

void hl::game::map::add_player(const std::shared_ptr<player> &player, const std::string &sp)
{

}

void hl::game::map::remove_player(uint64_t pid)
{

}

std::shared_ptr<hl::game::player> hl::game::map::find_player(uint64_t pid)
{
    return {};
}
