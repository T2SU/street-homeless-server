//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "world/map_state.hpp"
#include "net/master_session.hpp"
#include "users/user_record.hpp"
#include "world/game_world.hpp"

hl::master::map_state::map_state(uint32_t map_sn, uint32_t server_idx, map_type type, std::string scene)
    : _map_sn(map_sn)
    , _server_idx(server_idx)
    , _map_type(type)
    , _scene(std::move(scene))
    , _state(map_load_state::loading)
    , _queue()
    , _players()
{
}

void hl::master::map_state::add_player(std::shared_ptr<change_map_request> req)
{
    if (_state == map_load_state::existence)
    {
        _players.emplace(req->get_pid());
        flush(req, pb::ChangeMapResult_Success);
        LOGV << "added player to map. [" << _scene << "(" << _map_sn << ")]";
    }
    else
    {
        add_queue(std::move(req));
        LOGV << "added player to loading map queue. [" << _scene << "(" << _map_sn << ")]";
    }
}

void hl::master::map_state::remove_player(uint64_t pid)
{
    _players.erase(pid);
    LOGV << "remove player from map. [" << _scene << "(" << _map_sn << ")]";
}

void hl::master::map_state::add_queue(std::shared_ptr<change_map_request> req)
{
    _queue.push(std::move(req));
}

void hl::master::map_state::process_after_creation(bool success)
{
    LOGV << "process after creation of map. [" << _scene << "(" << _map_sn << ")]";
    while (!_queue.empty())
    {
        flush(_queue.front(), success ? pb::ChangeMapResult_Success : pb::ChangeMapResult_InternalServerError);
        _queue.pop();
    }
}

static bool encode_map_server_endpoint(uint32_t server_idx, uint32_t map_sn, out_buffer& obuf)
{
    const auto server = hl::singleton<hl::master::master_server>::get().get(server_type::game, server_idx);
    if (!server)
    {
        LOGV << "no game server for map " << map_sn << " (server idx " << server_idx << ")";
        return false;
    }
    obuf.write_str(server->get_endpoint_address());
    obuf.write(server->get_endpoint_port());
    return true;
}

void hl::master::map_state::flush(const std::shared_ptr<change_map_request>& req, pb::ChangeMapResult success)
{
    auto session = req->get_session();
    auto user = req->get_user();
    if (!session || !user)
    {
        LOGV << "failed to flush change_map_req cause session or user is offline. (pid: " << req->get_pid() << ") [" << _scene << "(" << _map_sn << ")]";
        return;
    }

    auto out_buf = req->make_reply(hl::InternalServerMessage_ChangeMapRes);
    out_buf.write(success);
    if (success == pb::ChangeMapResult_Success)
    {
        user->set_map_sn(_map_sn);
        if (!encode_map_server_endpoint(_server_idx, _map_sn, out_buf))
        {
            LOGV << "failed to encode map server " << _server_idx;
            out_buf = req->make_reply(hl::InternalServerMessage_ChangeMapRes);
            out_buf.write(pb::ChangeMapResult_InternalServerError);
        }
        out_buf.write(user->get_pid());
    }
    session->write(out_buf);

    LOGV << "flushed change_map_req (pid: " << req->get_pid() << ") (server_idx: " << session->get_idx() <<") [" << _scene << "(" << _map_sn << ")]";
}
