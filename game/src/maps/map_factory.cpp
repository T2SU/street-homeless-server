//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "maps/map_factory.hpp"
#include "maps/map.hpp"
#include "maps/portal.hpp"

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
        load_map(*map);
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

void hl::game::map_factory::load_map(hl::game::map& map)
{
    // TODO Redis caching

    const auto data_root = std::filesystem::path(hl::config::data_root);
    const auto path = data_root / "maps" / map.get_scene();

    const auto node = YAML::LoadFile(path.string() + ".yaml");
    const auto portals = node["portals"];
    for (const auto& pt : portals)
    {
        auto d = std::make_shared<portal>(pt);
        add_portal_to_map(map, d);
    }
}

void hl::game::map_factory::add_portal_to_map(hl::game::map& map, std::shared_ptr<portal> pt)
{
    map._portals.emplace(pt->get_name(), pt);
    if (pt->get_type() == portal_type::starting_point)
        map._starting_points.push_back(pt);
}