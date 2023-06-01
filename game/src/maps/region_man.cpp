//
// Created by TSU on 2023-06-01.
//

#include "std_common.hpp"
#include "maps/region_man.hpp"

hl::game::region_man::region_man()
    : _regions()
    , _mutex()
{}

void hl::game::region_man::create(region_sn_t sn, region_id_t id)
{
    synchronized (_mutex)
    {
        _regions.emplace(sn, std::make_unique<region>(id, sn));
    }
}

hl::game::region *hl::game::region_man::get(region_sn_t sn) const
{
    synchronized (_mutex)
    {
        auto it = _regions.find(sn);
        if (it != _regions.end())
            return it->second.get();
    }
    return nullptr;
}

void hl::game::region_man::remove(region_sn_t sn)
{
    synchronized (_mutex)
    {
        _regions.erase(sn);
    }
}
