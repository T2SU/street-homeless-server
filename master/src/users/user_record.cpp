//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "users/user_record.hpp"

hl::master::user_record::user_record(uint32_t server_idx, uint32_t socket_sn, uint64_t pid)
        : _pid(pid)
        , _server_idx(server_idx)
        , _socket_sn(socket_sn)
        , _state(user_state::migrating)
        , _player_data()
        , _db_job_state(db_job_state::waiting)
        , _jobs()
        , _map()
        , _mutex()
{}

const hl::player_data &hl::master::user_record::get_player_data() const
{
    return _player_data;
}

void hl::master::user_record::set_player_data(player_data&& player_data)
{
    _player_data = player_data;
}

user_state hl::master::user_record::get_state() const
{
    return _state;
}

void hl::master::user_record::set_state(user_state state)
{
    _state = state;
}

std::shared_ptr<hl::master::map_state> hl::master::user_record::get_map() const
{
    return _map;
}

void hl::master::user_record::set_map(std::shared_ptr<map_state> map)
{
    _map = std::move(map);
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
