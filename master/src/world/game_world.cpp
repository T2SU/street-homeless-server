//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "world/game_world.hpp"
#include "world/hangout.hpp"

size_t hl::master::game_world_dimension::calc_population() const
{
    auto populations = regions
                       | std::views::values
                       | std::views::filter([](const auto& region){ return region->size() > 0; })
                       | std::views::transform([](const auto& region){ return region->size() * (region->size() - 1); });
    return std::accumulate(populations.begin(), populations.end(), static_cast<size_t>(0));
}

hl::master::game_world::game_world()
    : _mutex()
    , _regions()
    , _dimensions()
    , _sn_counter()
{
}

/// public:

void hl::master::game_world::migrate_region(const std::shared_ptr<migration_region_request>& req)
{
    synchronized (_mutex)
    {
        // 기존 리전에서 삭제..
        auto user = req->get_user();
        remove_player(req->get_pid());

        // 새로운 리전 or 기존 리전
        const auto& scene = req->get_scene();
        const auto& map_data = hl::singleton<hl::map_data_s>::get().get(scene);
        const auto& region_info = hl::singleton<hl::region_info_s>::get().get(map_data.get_region_id());

        auto channels_it = _regions.find(region_info.get_region_id());
        if (channels_it == _regions.end())
            channels_it = _regions.emplace(region_info.get_region_id(), std::map<channel_id_t, std::shared_ptr<region>>()).first;

        auto channels = channels_it->second;

        // 채널 추가? or 기존 채널?
        const auto need_new_channel =
                channels.empty() ||
                std::ranges::all_of(channels | std::views::values, [](const auto& r){ return r->is_crowded(); });
        std::shared_ptr<region> region;
        if (need_new_channel)
        {
            // 새로운 채널 생성
            region_sn_t sn = ++_sn_counter;
            channel_id_t id = channels.empty() ? DefaultChannelId : (1 + *std::ranges::max_element(channels | std::views::keys));
            server_idx_t server_idx = retrieve_server_for_region(region_info.get_region_type());

            LOGV << "creating new region channel... [sn: " << sn << ", id: " << map_data.get_region_id() << ", channel id: " << id << "]";
            region = region::create(sn, id, server_idx, map_data, region_info);

            LOGV << "putting region channel... [sn: " << sn << ", id: " << map_data.get_region_id() << ", channel id: " << id << "]";
            put_region_to_game_world(region);

            LOGV << "requesting game world region creation [id: " << map_data.get_region_id() << "] to server " << server_idx;
            request_region_creation(region);
        }
        else
        {
            auto found = std::ranges::find_if_not(channels | std::views::values,
                                                  [](const auto& r){ return r->is_crowded(); },
                                                  [](const auto& r){ return r;});
            region = *found;
        }

        // 이제 리전에 사용자 추가..
        region->add_player(req);
    }
}

void hl::master::game_world::on_after_creation(server_idx_t server_idx, region_sn_t region_sn, bool success)
{
    synchronized (_mutex)
    {
        auto dimension = _dimensions.find(server_idx);
        if (dimension == _dimensions.end())
        {
            LOGV << "returned 1";
            return;
        }
        auto region_it = dimension->second.regions.find(region_sn);
        if (region_it == dimension->second.regions.end())
        {
            LOGV << "returned 2";
            return;
        }
        auto region = region_it->second;
        region->process_after_creation(success);
        if (success && !region->empty())
        {
            region->set_state(region_load_state::existence);
        }
        else
        {
            if (!success)
                LOGV << "failed to create region from game server - " << server_idx << ".. deleting region sn: " << region_sn;
            else
                LOGV << "created a region, but empty. - " << server_idx << ".. deleting region sn: " << region_sn;
            remove_region(region);
        }
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
        auto region = user->get_region();
        if (!region)
        {
            LOGD << "player (" << pid << ") has no region. do not anything.";
            return;
        }
        region->remove_player(pid);
        if (region->empty())
        {
            remove_region(region);
        }
    }
}

void hl::master::game_world::add_server(server_idx_t server_idx, uint32_t flag)
{
    synchronized (_mutex)
    {
        _dimensions.try_emplace(server_idx, game_world_dimension{.flag = flag});
    }
}

void hl::master::game_world::remove_server(uint32_t server_idx)
{
    synchronized (_mutex)
    {
        _dimensions.erase(server_idx);
    }
}

/// private:

void hl::master::game_world::put_region_to_game_world(const std::shared_ptr<region>& region)
{
    auto dimension_it = _dimensions.find(region->get_server_idx());
    if (dimension_it == _dimensions.end())
    {
        auto inserted = _dimensions.emplace(region->get_server_idx(), game_world_dimension());
        auto& dimension = inserted.first->second;
        dimension.regions.emplace(region->get_region_sn(), region);
    }
    else
    {
        dimension_it->second.regions.emplace(region->get_region_sn(), region);
    }
    _regions[region->get_region_id()].emplace(region->get_channel_id(), region);
}

void hl::master::game_world::request_region_creation(const std::shared_ptr<region> &region)
{
    auto session = hl::singleton<hl::master::master_server>::get().get(server_type::game, region->get_server_idx());
    if (!session)
    {
        LOGD << "game server " << region->get_server_idx() << " is down.";
        return;
    }

    out_buffer obuf(hl::InternalServerMessage_RegionManagementReq);
    obuf.write(region_management_type::create);
    obuf.write(region->get_region_sn());
    obuf.write(region->get_region_id());
    session->write(obuf);
}


void hl::master::game_world::remove_region(const std::shared_ptr<region>& region)
{
    LOGV << "[REMOVING] [server: " << region->get_server_idx() << "] region id: " << region->get_region_id() << ", region sn: " << region->get_region_sn() << ", channel id: " << region->get_channel_id();
    _dimensions[region->get_server_idx()].regions.erase(region->get_region_sn());

    auto& channels = _regions[region->get_region_id()];
    channels.erase(region->get_channel_id());

    // 이거 마스터에서 리전 삭제를 시도하니까
    // [마스터->게임]으로 RegionRemove 요청이 간 후에야,
    // [유저->게임] 연결이 끊기는 이벤트가 도착하기 때문에 게임서버쪽에서 액세스 위반이 일어남.
    //  ->  따라서, 게임쪽에서 알아서 빈 region을 해제하는게 나을 것 같음.

//    out_buffer obuf(hl::InternalServerMessage_RegionManagementReq);
//    obuf.write(region_management_type::remove);
//    obuf.write(region->get_region_sn());
//
//    auto session = hl::singleton<hl::master::master_server>::get().get(server_type::game, region->get_server_idx());
//    if (session)
//        session->write(obuf);
}

server_idx_t hl::master::game_world::retrieve_server_for_region(region_type type) const
{
    auto flag = type == region_type::regular ? game_server_flag::game_field : game_server_flag::game_instance;
    auto possible_servers = _dimensions
                            | std::views::filter([flag](const auto& pair){ return pair.second.flag & flag; });
    if (possible_servers.empty())
        return 0;
    return std::ranges::min_element(possible_servers.begin(), possible_servers.end(),
                                    [](const auto& pair1, const auto& pair2){ return pair1.second.calc_population() < pair2.second.calc_population(); })
            ->first;
}
