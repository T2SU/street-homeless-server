//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "maps/map.hpp"
#include "users/player.hpp"
#include "net/master.hpp"

hl::game::map::map(uint32_t map_sn, std::string scene, map_type type)
        : _map_sn(map_sn), _scene(std::move(scene)), _type(type)
        , _portals(), _starting_points()
        , _mutex(), _players(), _objects()
{
}

uint32_t hl::game::map::get_map_sn() const
{
    return _map_sn;
}

const std::string &hl::game::map::get_scene() const
{
    return _scene;
}

map_type hl::game::map::get_type() const
{
    return _type;
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

void hl::game::map::remove_player(uint64_t pid)
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
        out_buffer req(hl::InternalClientMessage_ChangeMapReq);
        req.write(player->get_session()->get_socket_sn());
        req.write_str(device_id);
        req.write_str(player->get_session()->get_remote_address());
        req.write(player->get_pid());
        req.write_str(pt->second->get_target_scene());
        req.write_str(pt->second->get_target_portal_name());
        MASTER->write(req);
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
    for (auto player : get_players())
    {
        if (player->get_pid() == exception_pid)
            continue;
        player->get_session()->write(out);
    }
}