//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "world/region.hpp"
#include "net/master_session.hpp"
#include "users/user_record.hpp"
#include "world/game_world.hpp"
#include "../../../game/include/maps/map_factory.hpp"

hl::master::region::region(region_sn_t region_sn, channel_id_t channel_id, server_idx_t server_idx, const map_data& map_data, const region_info& region_info)
    : enable_shared_from_this()
    , _region_id(map_data.get_region_id())
    , _region_sn(region_sn)
    , _region_type(region_info.get_region_type())
    , _max_players(region_info.get_max_players())
    , _channel_id(channel_id)
    , _server_idx(server_idx)
    , _state(region_load_state::loading)
    , _queue()
    , _players()
{
}

void hl::master::region::add_player(std::shared_ptr<migration_region_request> req)
{
    if (_state == region_load_state::existence)
    {
        _players.emplace(req->get_pid());
        flush(req, pb::ChangeMapResult_Success);
        LOGV << "added player to region. [region sn: " << _region_sn << ", region id: " << _region_id << "]";
    }
    else
    {
        add_queue(std::move(req));
        LOGV << "added player to loading region queue. [region sn: " << _region_sn << ", region id: " << _region_id << "]";
    }
}

void hl::master::region::remove_player(player_id_t pid)
{
    _players.erase(pid);
    LOGV << "remove player from map. [region sn: " << _region_sn << ", region id: " << _region_id << "]";
}

void hl::master::region::add_queue(std::shared_ptr<migration_region_request> req)
{
    _queue.push(std::move(req));
}

void hl::master::region::process_after_creation(bool success)
{
    LOGV << "process after creation of map. [region sn: " << _region_sn << ", region id: " << _region_id << "]";
    while (!_queue.empty())
    {
        auto& req = _queue.front();
        flush(req, success ? pb::ChangeMapResult_Success : pb::ChangeMapResult_InternalServerError);
        _players.emplace(req->get_pid());
        _queue.pop();
    }
}

static bool encode_map_server_endpoint(server_idx_t server_idx, out_buffer& obuf)
{
    const auto server = hl::singleton<hl::master::master_server>::get().get(server_type::game, server_idx);
    if (!server) return false;
    obuf.write_str(server->get_endpoint_address());
    obuf.write(server->get_endpoint_port());
    return true;
}

void hl::master::region::flush(const std::shared_ptr<migration_region_request>& req, pb::ChangeMapResult result)
{
    auto session = req->get_session();
    auto user = req->get_user();
    if (!session || !user)
    {
        LOGV << "failed to flush migration_region_req cause session or user is offline. (pid: " << req->get_pid() << ") [region sn: " << _region_sn << ", region id: " << _region_id << "]";
        return;
    }
    req->get_user()->set_server_idx(session->get_idx());

    auto out_buf = req->make_reply(hl::InternalServerMessage_MigrateRegionRes);
    out_buf.write(req->get_pid());
    out_buf.write(result);
    if (result == pb::ChangeMapResult_Success)
    {
        user->set_region(shared_from_this());
        if (!encode_map_server_endpoint(_server_idx, out_buf))
        {
            LOGV << "failed to encode map server " << _server_idx;
            out_buf = req->make_reply(hl::InternalServerMessage_MigrateRegionRes);
            out_buf.write(req->get_pid());
            out_buf.write(pb::ChangeMapResult_InternalServerError);
        }
    }
    session->write(out_buf);

    LOGV << "flushed migration_region_req (pid: " << req->get_pid() << ") (server_idx: " << session->get_idx() <<") [region sn: " << _region_sn << ", region id: " << _region_id << "]";
}