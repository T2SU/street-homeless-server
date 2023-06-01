//
// Created by TSU on 2023-05-14.
//


#include "std.hpp"
#include "mhandlers/on_migrate_res.hpp"
#include "users/player.hpp"
#include "maps/map.hpp"
#include "maps/map_factory.hpp"
#include "maps/region_man.hpp"

void hl::game::mhandlers::on_migrate_res::handle_packet(master &session, in_buffer &in_buf)
{
    const auto socket_sn = in_buf.read<uint32_t>();
    const auto success = in_buf.read<bool>();

    out_buffer out(pb::ServerMessage_SetMap);
    auto p = hl::singleton<hl::game::game_server>::get().find(socket_sn);
    if (!p)
    {
        LOGD << "socket " << socket_sn << " is not present.";
        return;
    }
    if (!success)
    {
        LOGD << "socket " << socket_sn << " has failed to enter game world";
        out.write<uint8_t>(0);
        p->write(out);
        return;
    }

    pb::PlayerData pb{};
    const auto pid = in_buf.read<player_id_t>();
    const auto region_sn = in_buf.read<region_sn_t>();
    const auto scene = in_buf.read_str();
    auto pt = in_buf.read_str();
    const auto first_enter = in_buf.read<uint8_t>();
    in_buf.read_pb(pb);

    p->set_state(user_state::connected);

    auto region = REGIONS.get(region_sn);
    if (region == nullptr)
    {
        LOGD << "region sn " << region_sn << " does not initialized.";
        out.write<uint8_t>(0);
        p->write(out);
        return;
    }
    auto map = region->maps().get_map(scene);
    if (map == nullptr)
    {
        LOGD << "scene " << scene << " cannot be initialized.";
        out.write<uint8_t>(0);
        p->write(out);
        return;
    }

    auto player = std::make_shared<hl::game::player>(pid, p);
    player->init(pb, map);
    p->set_player(player);
    map->put_on_portal(player, pt);

    out.write<uint8_t>(1);
    out.write(pid);
    out.write_str(scene);
    out.write_str(pt);
    out.write(first_enter);
    if (first_enter)
    {
        out.write_pb(pb);
    }
    p->write(out);

    map->add_player(player);
}
