//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "maps/map.hpp"
#include "users/player.hpp"
#include "net/master.hpp"
#include "maps/region_man.hpp"

hl::game::map::map(std::string scene, region_id_t id, region_sn_t sn)
        : _scene(std::move(scene)), _region_id(id), _region_sn(sn)
        , _portals(), _starting_points()
        , _mutex(), _players(), _objects()
{
}

const std::string &hl::game::map::get_scene() const
{
    return _scene;
}

region_id_t hl::game::map::get_region_id() const
{
    return _region_id;
}

region_sn_t hl::game::map::get_region_sn() const
{
    return _region_sn;
}

void hl::game::map::put_on_portal(const std::shared_ptr<player>& player, std::string& sp)
{
    const auto& pt = get_portal(sp);
    player->set_position(pt->get_pos());
    player->set_rotation(pt->get_rot());
    sp = pt->get_name();
}

void hl::game::map::add_player(const std::shared_ptr<player> &player)
{
    synchronized(_mutex)
    {
        for (const auto& obj : get_objects())
            player->send_enter(obj);
        for (const auto& p : get_players())
        {
            player->send_enter(std::reinterpret_pointer_cast<field_object>(p));
            p->send_enter(std::reinterpret_pointer_cast<field_object>(player));
        }
        _players[player->get_pid()] = player;
    }
}

void hl::game::map::remove_player(player_id_t pid)
{
    synchronized(_mutex)
    {
        auto found = _players.find(pid);
        if (found != _players.end())
        {
            for (const auto& p : get_players())
            {
                if (p->get_pid() == pid)
                    continue;
                p->send_leave(std::reinterpret_pointer_cast<field_object>(found->second));
            }
            _players.erase(pid);
        }
    }
    if (REGIONS.get(get_region_sn())->maps().empty())
    {
        LOGV << "region " << get_region_sn() << " is empty. deleteing current region...";
        REGIONS.remove(get_region_sn());
    }
}

std::shared_ptr<hl::game::player> hl::game::map::find_player(uint64_t pid)
{
    const auto it = _players.find(pid);
    if (it != _players.end())
        return it->second;
    return {};
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
std::shared_ptr<hl::game::portal> hl::game::map::get_portal(const std::string &pt) const
{
    if (pt.empty())
    {
        return _starting_points[RAND.next<size_t>(_starting_points.size() - 1)];
    }
    else
    {
        const auto& found = _portals.find(pt);
        if (found != _portals.end())
        {
            return found->second;
        }
    }
    // fallback invalid name
    return get_portal("");
}
#pragma clang diagnostic pop

std::shared_ptr<hl::game::field_object> hl::game::map::find_object_no_lock(uint64_t object_id) const
{
    auto found = _objects.find(object_id);
    if (found != _objects.end())
    {
        return found->second;
    }
    auto found_user = _players.find(object_id);
    if (found_user != _players.end())
    {
        return std::reinterpret_pointer_cast<field_object>(found_user->second);
    }
    return {};
}

void hl::game::map::on_move_player(const std::shared_ptr<player> &player, in_buffer &in)
{
    synchronized (_mutex)
    {
        // TODO Check movement speed and validity

        const auto move_req_type = static_cast<pb::MoveRequestType>(in.read<uint8_t>());
        const auto pos = in.read_pb<pb::Vector3>();
        player->set_position(pos);
        if (move_req_type == pb::MoveRequestType::SetDestination)
        {
            out_buffer out(pb::ServerMessage_MovePlayer);
            out.write(player->get_pid());
            out.write_pb(pos);
            broadcast(out, player->get_pid());
        }
        else if (move_req_type == pb::MoveRequestType::TargetReached)
        {
            const auto rot = in.read_pb<pb::Quaternion>();
            player->set_rotation(rot);
        }
    }
}

void hl::game::map::on_change_map_req(const std::shared_ptr<player>& player, in_buffer& in)
{
    const auto portal_name = in.read_str();
    const auto pos = in.read_pb<pb::Vector3>();
    const auto device_id = in.read_str();

    // TODO check player <-> portal distance (anti hacking)
    const auto& pt = _portals.find(portal_name);
    if (pt == _portals.end())
    {
        out_buffer out(pb::ServerMessage_ChangeMapRes);
        out.write<uint8_t>(pb::ChangeMapResult_UnknownError);
        player->get_session()->write(out);
        return;
    }
    if (pt->second->get_type() == portal_type::change_map)
    {
        const auto& target_data = hl::singleton<map_data_s>::get().get(pt->second->get_target_scene());
        if (target_data.get_region_id() != _region_id)
        {
            request_migrate_region(player, pt->second, device_id);
        }
        else
        {
            change_map_local(player, pt->second);
        }
    }
    else if (pt->second->get_type() == portal_type::script) // TODO 아직 미구현..
    {
        out_buffer out(pb::ServerMessage_ChangeMapRes);
        out.write<uint8_t>(pb::ChangeMapResult_InternalServerError);
        player->get_session()->write(out);
        return;
    }
}

void hl::game::map::broadcast(const out_buffer& out, uint64_t exception_pid)
{
    for (auto& player : get_players())
    {
        if (player->get_pid() == exception_pid)
            continue;
        player->get_session()->write(out);
    }
}

void hl::game::map::change_map_local(const std::shared_ptr<player> &player, const std::shared_ptr<portal> &pt) const
{
    out_buffer out(pb::ServerMessage_SetMap);
    auto map = REGIONS.get(get_region_sn())->maps().get_map(pt->get_target_scene());
    if (map == nullptr)
    {
        out_buffer e(pb::ServerMessage_ChangeMapRes);
        e.write<uint8_t>(pb::ChangeMapResult_UnknownError);
        player->get_session()->write(e);
        return;
    }
    auto pt_name = pt->get_target_portal_name();
    auto previous_map = player->get_map();

    player->set_map(map);
    map->put_on_portal(player, pt_name);
    map->add_player(player);

    if (previous_map != nullptr)
        previous_map->remove_player(player->get_pid());

    out.write<uint8_t>(1); // success
    out.write(player->get_pid());
    out.write_str(map->get_scene());
    out.write_str(pt_name);
    out.write<uint8_t>(0); // first enter
    player->get_session()->write(out);
}


void hl::game::map::request_migrate_region(const std::shared_ptr<player>& player, const std::shared_ptr<portal>& pt, const std::string& device_id)
{
    out_buffer req(hl::InternalClientMessage_MigrateRegionReq);
    req.write(player->get_session()->get_socket_sn());
    req.write_str(device_id);
    req.write_str(player->get_session()->get_remote_address());
    req.write(player->get_pid());
    req.write_str(pt->get_target_scene());
    req.write_str(pt->get_target_portal_name());
    MASTER->write(req);
}
