//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "world/map_state.hpp"
#include "net/master_session.hpp"

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

void hl::master::map_state::add_player(std::shared_ptr<change_map_req> req)
{
    if (_state == map_load_state::existence)
    {
        _players.emplace(req->get_pid());
        flush(req);
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

void hl::master::map_state::add_queue(std::shared_ptr<change_map_req> req)
{
    _queue.push(std::move(req));
}

void hl::master::map_state::process_after_creation()
{
    LOGV << "process after creation of map. [" << _scene << "(" << _map_sn << ")]";
    while (!_queue.empty())
    {
        flush(_queue.front());
        _queue.pop();
    }
}

void hl::master::map_state::flush(const std::shared_ptr<change_map_req>& req)
{
    auto session = req->get_session();
    auto user = req->get_user();
    if (!session || !user)
    {
        LOGV << "failed to flush change_map_req cause session or user is offline. (pid: " << req->get_pid() << ") [" << _scene << "(" << _map_sn << ")]";
        return;
    }

    auto out_buf = req->make_reply(hl::InternalServerMessage_EnterGameRes);
    out_buf.write(req->is_first_enter());
    out_buf.write_str(session->get_endpoint_address());
    out_buf.write(session->get_endpoint_port());
    out_buf.write_str(req->get_scene());
    out_buf.write_str(req->get_starting_point());
    session->write(out_buf);

    LOGV << "flushed change_map_req (pid: " << req->get_pid() << ") (server_idx: " << session->get_idx() <<") [" << _scene << "(" << _map_sn << ")]";
}
