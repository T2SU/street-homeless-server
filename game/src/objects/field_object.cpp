//
// Created by T2SU on 2023/05/16.
//

#include "std.hpp"
#include "objects/field_object.hpp"

hl::game::field_object::field_object(uint64_t object_id)
    : _object_id(object_id)
{
}

hl::game::field_object::~field_object()
{
}

uint64_t hl::game::field_object::get_id() const
{
    return _object_id;
}

std::string hl::game::field_object::to_string() const
{
    return fmt::format("[object_id={}]", _object_id);
}
