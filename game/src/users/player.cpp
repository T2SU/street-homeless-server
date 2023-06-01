//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "users/player.hpp"
#include "maps/map_factory.hpp"
#include "maps/map.hpp"

hl::game::player::player(uint64_t pid, const std::shared_ptr<game_session> session)
    : field_object(pid)
    , _pid(pid)
    , _uid()
    , _stat()
    , _money()
    , _inventory()
    , _name()
    , _map()
    , _position()
    , _rotation()
    , _session(session)
{
}

void hl::game::player::init(pb::PlayerData &data, map* map)
{
    _pid = data.pid();
    _object_id = data.pid();
    _uid = data.uid();
    _name = data.name();
    _map = map;
    _stat.init(data);
    init_inventory(data);
}

void hl::game::player::init_inventory(pb::PlayerData &data)
{
    _money = data.inventory().money();
    for (const auto& element : data.inventory().items())
    {
        _inventory.emplace_back(element.item_id(), element.amount());
    }
}

void hl::game::player::on_close()
{
    // TODO close actions

    auto map = get_map();
    if (map != nullptr)
    {
        map->remove_player(_pid);
    }
}

uint64_t hl::game::player::get_pid() const
{
    return _pid;
}

uint64_t hl::game::player::get_uid() const
{
    return _uid;
}

const std::string &hl::game::player::get_name() const
{
    return _name;
}

std::shared_ptr<hl::game::game_session> hl::game::player::get_session() const
{
    return _session.lock();
}

const hl::vector3 &hl::game::player::get_position() const
{
    return _position;
}

void hl::game::player::set_position(hl::vector3 position)
{
    _position = position;
}

const hl::quaternion &hl::game::player::get_rotation() const
{
    return _rotation;
}

void hl::game::player::set_rotation(hl::quaternion rotation)
{
    _rotation = rotation;
}

void hl::game::player::encode_enter(out_buffer &out) const
{
    out.write<uint16_t>(pb::ServerMessage_EnterPlayer);
    out.write(_pid);
    out.write_str(_name);
    out.write_pb(_stat.get_appearance());
    out.write_pb(static_cast<pb::Vector3>(_position));
    out.write_pb(static_cast<pb::Quaternion>(_rotation));
}

void hl::game::player::encode_leave(out_buffer &out) const
{
    out.write<uint16_t>(pb::ServerMessage_LeavePlayer);
    out.write(_pid);
}

void hl::game::player::send_enter(const std::shared_ptr<field_object>& obj)
{
    LOGV << "Send field_obj '" << obj->to_string() << "' enter message to player " << _name << "(" << _pid << ")";

    out_buffer out;
    obj->encode_enter(out);
    get_session()->write(out);
}

void hl::game::player::send_leave(const std::shared_ptr<field_object>& obj)
{
    LOGV << "Send field_obj '" << obj->to_string() << "' leave message to player " << _name << "(" << _pid << ")";

    out_buffer out;
    obj->encode_leave(out);
    get_session()->write(out);
}

// 맵은 마스터서버에서만 삭제 요청을 할 수 있음.
// 마스터서버에서 맵 삭제 요청이 오려면, 해당 맵에 유저가 아무도 없어야 함.
// 따라서 이론적으로, 해당 맵에 아무도 없다는 것이 보장될 수 있음. 따라서 공유 포인터를 받을 필요가 없고 unique_ptr로 처리 가능.
hl::game::map* hl::game::player::get_map() const
{
    return _map;
}

void hl::game::player::set_map(hl::game::map *map)
{
    _map = map;
}

std::string hl::game::player::to_string() const
{
    return fmt::format("[pid={} name={}]", _pid, _name);
}
