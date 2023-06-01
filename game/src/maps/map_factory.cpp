//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "maps/map_factory.hpp"
#include "maps/map.hpp"
#include "maps/portal.hpp"

hl::game::map_factory::map_factory(region_id_t id, region_sn_t sn)
        : _id(id), _sn(sn), _mutex(), _maps()
{
}

bool hl::game::map_factory::create_map(const std::string &scene)
{
    std::unique_ptr<hl::game::map> map;

    if (!scene.empty())
    {
        LOGV << "creating map... scene:" << scene;
        map = std::make_unique<hl::game::map>(scene, get_region_id(), get_region_sn());
        load_map(*map);
    }
    if (!map)
    {
        LOGE << "failed to create map '" << scene << "'";
        return false;
    }
    _maps.emplace(scene, std::move(map));
    LOGV << "created map! scene:" << scene;
    return true;
}

void hl::game::map_factory::remove_map(const std::string& scene)
{
    synchronized (_mutex)
    {
        _maps.erase(scene);
    }
    LOGV << "removed map! scene:" << scene;
}

hl::game::map *hl::game::map_factory::get_map(const std::string& scene)
{
    synchronized (_mutex)
    {
        auto it = _maps.find(scene);
        if (it != _maps.end())
            return it->second.get();
        if (create_map(scene))
            return _maps[scene].get();
    }
    return nullptr;
}

void hl::game::map_factory::load_map(hl::game::map& map)
{
    // TODO Redis caching?

    const auto data = hl::map_data(map.get_scene());
    const auto portals = data["portals"];
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

region_id_t hl::game::map_factory::get_region_id() const
{
    return _id;
}

region_sn_t hl::game::map_factory::get_region_sn() const
{
    return _sn;
}
