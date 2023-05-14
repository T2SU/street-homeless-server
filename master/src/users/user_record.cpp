//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "users/user_record.hpp"

hl::master::user_record::user_record(uint32_t server_idx, uint32_t socket_sn, uint64_t pid, std::string device_id, std::string remote_address)
        : _pid(pid)
        , _server_idx(server_idx)
        , _socket_sn(socket_sn)
        , _state(user_state::migrating)
        , _player_data()
        , _db_job_state(db_job_state::waiting)
        , _jobs()
        , _map_sn()
        , _mutex()
        , _device_id(std::move(device_id))
        , _remote_address(std::move(remote_address))
        , _migration_count(0)
{}

const hl::player_data &hl::master::user_record::get_player_data() const
{
    return _player_data;
}

void hl::master::user_record::set_player_data(player_data&& player_data)
{
    _player_data = player_data;
}

void hl::master::user_record::set_new_map(const std::string& scene, const std::string& sp)
{
    _player_data.set_map(scene);
    _player_data.set_sp(sp);
}

user_state hl::master::user_record::get_state() const
{
    return _state;
}

void hl::master::user_record::set_state(user_state state)
{
    _state = state;
}

void hl::master::user_record::process_db_job(sqlpp::mysql::connection &conn)
{
    while (true)
    {
        std::shared_ptr<hl::database::job> job;
        synchronized (_mutex)
        {
            if (_jobs.empty())
            {
                _db_job_state = db_job_state::waiting;
                return;
            }
            job = _jobs.front();
            _jobs.pop();
        }
        job->process(conn);
    }
}

uint32_t hl::master::user_record::get_map_sn() const
{
    return _map_sn;
}

void hl::master::user_record::set_map_sn(uint32_t map_sn)
{
    _map_sn = map_sn;
}

const std::string &hl::master::user_record::get_device_id() const
{
    return _device_id;
}

const std::string &hl::master::user_record::get_remote_address() const
{
    return _remote_address;
}
