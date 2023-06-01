//
// Created by TSU on 2023-06-01.
//

#include "std.hpp"
#include "maps/region.hpp"

hl::game::region::region(const region_id_t id, const region_sn_t sn)
        : _id(id), _sn(sn), _maps(id, sn)
{}
