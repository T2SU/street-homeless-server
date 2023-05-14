//
// Created by TSU on 2023-05-14.
//

#include "std_common.hpp"
#include "users/player_data.hpp"
#include "homeless-db.h"

hl::player_data::player_data()
    : _data(), _appearance(), _inventory(), _player_stat(), _map(), _sp()
{
    _data.set_allocated_stat(&_player_stat);
    _data.set_allocated_inventory(&_inventory);
    _data.set_allocated_appearance(&_appearance);
}

void hl::player_data::load(uint64_t pid, sqlpp::mysql::connection &conn)
{
    db::Characters chr{};

    const auto user_result = conn(select(
                            chr.pid,
                            chr.uid,
                            chr.name,
                            chr.health,
                            chr.tiredness,
                            chr.maxHealth,
                            chr.maxTiredness,
                            chr.hair,
                            chr.hairColor,
                            chr.shirt,
                            chr.pants,
                            chr.shoes,
                            chr.hat,
                            chr.beard,
                            chr.gender,
                            chr.map,
                            chr.startingPoint,
                            chr.fat,
                            chr.muscle,
                            chr.slimness,
                            chr.breast,
                            chr.money)
                             .from(chr)
                             .where(chr.pid == pid));
    const auto& user_row = user_result.front();

    _data.set_uid(user_row.pid);
    _data.set_uid(user_row.uid);
    _data.set_name(user_row.name);
    _player_stat.set_health(static_cast<int32_t>(user_row.health));
    _player_stat.set_tiredness(static_cast<int32_t>(user_row.tiredness));
    _player_stat.set_max_health(static_cast<int32_t>(user_row.maxHealth));
    _player_stat.set_max_tiredness(static_cast<int32_t>(user_row.maxTiredness));
    _appearance.set_hair(static_cast<int32_t>(user_row.hair));
    _appearance.set_hair_color(static_cast<int32_t>(user_row.hairColor));
    _appearance.set_shirt_index(static_cast<int32_t>(user_row.shirt));
    _appearance.set_pants_index(static_cast<int32_t>(user_row.pants));
    _appearance.set_shoes_index(static_cast<int32_t>(user_row.shoes));
    _appearance.set_hat_index(static_cast<int32_t>(user_row.hat));
    _appearance.set_beard(static_cast<int32_t>(user_row.beard));
    _appearance.set_gender(static_cast<int32_t>(user_row.gender));
    _appearance.set_fat(static_cast<float>(user_row.fat));
    _appearance.set_muscles(static_cast<float>(user_row.muscle));
    _appearance.set_slimness(static_cast<float>(user_row.slimness));
    _appearance.set_breast(static_cast<float>(user_row.breast));
    _map = user_row.map;
    _sp = user_row.startingPoint;
    _inventory.set_money(static_cast<int32_t>(user_row.money));

    db::Inventory inv{};
    auto inventory_result = conn(select(inv.sn, inv.itemid, inv.amount)
            .from(inv)
            .where(inv.pid == pid));

    for (const auto& inv_row : inventory_result)
    {
        auto item = _inventory.add_items();
        item->set_sn(inv_row.sn);
        item->set_item_id(static_cast<int32_t>(inv_row.itemid));
        item->set_amount(static_cast<int32_t>(inv_row.amount));
    }

    LOGV << "loaded player data - " << pid << " from database";
}

const std::string &hl::player_data::get_map()
{
    return _map;
}

const std::string &hl::player_data::get_sp()
{
    return _sp;
}
