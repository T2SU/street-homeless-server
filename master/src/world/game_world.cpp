//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "world/game_world.hpp"
#include "world/hangout.hpp"

size_t hl::master::game_world_dimension::calc_population() const
{
    auto populations = maps
                       | std::views::filter([](const auto& pair){ return pair.second->size() > 0; })
                       | std::views::transform([](const auto& pair){ return pair.second->size() * (pair.second->size() - 1); });
    return std::accumulate(populations.begin(), populations.end(), static_cast<size_t>(0));
}

hl::master::game_world::game_world()
    : _mutex()
    , _fields()
    , _instances()
    , _maps_of_servers()
    , _sn_counter()
{
}

static map_type get_map_type(const std::string& scene)
{
    if (!scene.empty())
        return map_type::field; // TODO get real map type from game data
    return map_type::instance;
}

/// public:

void hl::master::game_world::change_map(const std::shared_ptr<change_map_request>& req)
{
    synchronized (_mutex)
    {
        const auto& scene = req->get_scene();
        const auto map_type = get_map_type(scene);

        std::shared_ptr<map_state> map;
        if (map_type == map_type::field)
        {
            auto field = _fields.find(scene);
            if (field == _fields.end())
            {
                create_map(req);
            }
            else
            {
                field->second->add_player(req);
            }
        }
        else
        {
            auto instance = _instances.find(scene);
            if (instance == _instances.end())
            {
                create_map(req);
            }
            else
            {
                instance->second->add_player(req);
            }
        }
    }
}

void hl::master::game_world::on_after_creation(uint32_t server_idx, uint32_t map_sn)
{
    synchronized (_mutex)
    {
        auto dimension = _maps_of_servers.find(server_idx);
        if (dimension == _maps_of_servers.end())
        {
            LOGV << "returned";
            return;
        }
        auto map_it = dimension->second.maps.find(map_sn);
        if (map_it == dimension->second.maps.end())
        {
            LOGV << "returned";
            return;
        }
        map_it->second->process_after_creation();
    }
}

void hl::master::game_world::remove_player(uint64_t pid)
{
    auto user = hl::singleton<hl::master::hangout>::get().find_user(pid);
    if (!user)
    {
        LOGD << "cannot remove player (" << pid << ") from game_world because user_record is not present.";
        return;
    }
    synchronized (_mutex)
    {
        auto dimension = _maps_of_servers.find(user->get_server_idx());
        if (dimension == _maps_of_servers.end())
        {
            LOGD << "cannot remove player (" << pid << ") from game_world because server idx " << user->get_server_idx() << " has no dimension.";
            return;
        }
        auto map_it = dimension->second.maps.find(user->get_map_sn());
        if (map_it == dimension->second.maps.end())
        {
            LOGD << "cannot remove player (" << pid << ") from game_world because server idx " << user->get_server_idx() << " doesn't have map sn " << user->get_map_sn();
            return;
        }
        auto map = map_it->second;
        map->remove_player(pid);
        if (map->empty())
        {
            remove_map(map);
        }
    }
}

void hl::master::game_world::add_server(uint32_t server_idx, uint32_t flag)
{
    synchronized (_mutex)
    {
        _maps_of_servers.try_emplace(server_idx, game_world_dimension{.flag = flag});
    }
}

void hl::master::game_world::remove_server(uint32_t server_idx)
{
    synchronized (_mutex)
    {
        _maps_of_servers.erase(server_idx);
    }
}


/// private:

void hl::master::game_world::create_map(const std::shared_ptr<change_map_request>& req)
{
    const auto map_type = get_map_type(req->get_scene());
    const auto server_idx = retrieve_server_for_map(map_type);
    auto new_map = std::make_shared<map_state>(++_sn_counter, server_idx, map_type, req->get_scene());
    if (map_type == map_type::field)
    {
        _fields.emplace(req->get_scene(), new_map).first;
    }
    else
    {
        _instances.emplace(req->get_scene(), new_map).first;
    }
    put_map(new_map);
    request_map_creation(new_map);
}

void hl::master::game_world::put_map(const std::shared_ptr<map_state>& map)
{
    auto maps_of_server_it = _maps_of_servers.find(map->get_server_idx());
    if (maps_of_server_it == _maps_of_servers.end())
    {
        auto inserted = _maps_of_servers.emplace(map->get_server_idx(), game_world_dimension());
        auto& maps_of_server = inserted.first->second;
        maps_of_server.maps.emplace(map->get_map_sn(), map);
    }
    else
    {
        maps_of_server_it->second.maps.emplace(map->get_map_sn(), map);
    }
}

void hl::master::game_world::request_map_creation(const std::shared_ptr<map_state> &map)
{
    auto session = hl::singleton<hl::master::master_server>::get().get(server_type::game, map->get_server_idx());
    if (!session)
    {
        LOGD << "game server " << map->get_server_idx() << " is down.";
        return;
    }

    out_buffer obuf(hl::InternalServerMessage_MapManagementReq);
    obuf.write(map_management_type::create);
    obuf.write(map->get_map_sn());
    obuf.write_str(map->get_scene());
    obuf.write(map->get_map_type());
    session->write(obuf);
}


void hl::master::game_world::remove_map(const std::shared_ptr<map_state>& map)
{
    auto maps_of_server_it = _maps_of_servers.find(map->get_server_idx());
    if (maps_of_server_it != _maps_of_servers.end())
    {
        maps_of_server_it->second.maps.erase(map->get_map_sn());
    }
}

uint32_t hl::master::game_world::retrieve_server_for_map(map_type type) const
{
    auto flag = type == map_type::field ? game_server_flag::game_field : game_server_flag::game_instance;
    auto possible_servers = _maps_of_servers
                            | std::views::filter([flag](const auto& pair){ return pair.second.flag & flag; });
    if (possible_servers.empty())
        return 0;
    return std::ranges::min_element(possible_servers.begin(), possible_servers.end(),
                                    [](const auto& pair1, const auto& pair2){ return pair1.second.calc_population() < pair2.second.calc_population(); })
            ->first;
}
