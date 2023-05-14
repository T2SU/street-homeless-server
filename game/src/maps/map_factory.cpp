//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "maps/map_factory.hpp"

hl::game::map_factory::map_factory()
        : _mutex(), _maps()
{
}

bool hl::game::map_factory::create_map(uint32_t map_sn, const std::string &scene, map_type type)
{
    std::unique_ptr<hl::game::map> map;

    if (!scene.empty())
    {
        LOGV << "creating map... sn:" << map_sn << ", scene:" << scene;
        map = std::make_unique<hl::game::map>(map_sn, scene, type);
    }
    if (!map)
    {
        LOGE << "failed to create map '" << scene << "'";
        return false;
    }
    synchronized (_mutex)
    {
        _maps.emplace(map_sn, std::move(map));
    }
    LOGV << "created map! sn:" << map_sn << ", scene:" << scene;
    return true;
}

void hl::game::map_factory::remove_map(uint32_t map_sn)
{
    synchronized (_mutex)
    {
        _maps.erase(map_sn);
    }
    LOGV << "removed map! sn:" << map_sn;
}

hl::game::map *hl::game::map_factory::get_map(uint32_t map)
{
    synchronized (_mutex)
    {
        auto it = _maps.find(map);
        if (it == _maps.end()) return nullptr;
        return it->second.get();
    }
    return nullptr;
}
