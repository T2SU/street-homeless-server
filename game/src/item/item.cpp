//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "items/item.hpp"

hl::game::item::item(int32_t item_id, int32_t amount)
        : _item_id(item_id), _amount(amount)
{}

uint64_t hl::game::item::get_sn() const
{
    return _sn;
}

void hl::game::item::set_sn(uint64_t sn)
{
    _sn = sn;
}

int32_t hl::game::item::get_item_id() const
{
    return _item_id;
}

void hl::game::item::set_item_id(int32_t item_id)
{
    _item_id = item_id;
}

int32_t hl::game::item::get_amount() const
{
    return _amount;
}

void hl::game::item::set_amount(int32_t amount)
{
    _amount = amount;
}

std::ostream &hl::game::operator<<(std::ostream &os, const hl::game::item &item)
{
    os << "[" << item.get_item_id() << ", " << item.get_amount() << "]";
    return os;
}
