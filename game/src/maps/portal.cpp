//
// Created by T2SU on 2023/05/16.
//

#include "std.hpp"
#include "maps/portal.hpp"

hl::game::portal::portal(const uint32_t sn, const std::string &name, const hl::vector3 &pos, const hl::quaternion &rot)
        : _sn(sn), _name(name), _pos(pos), _rot(rot)
{}

uint32_t hl::game::portal::get_sn() const
{
    return _sn;
}

const std::string &hl::game::portal::get_name() const
{
    return _name;
}

const hl::vector3 &hl::game::portal::get_pos() const
{
    return _pos;
}

const hl::quaternion &hl::game::portal::get_rot() const
{
    return _rot;
}
