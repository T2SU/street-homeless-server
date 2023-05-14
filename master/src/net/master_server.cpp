//
// Created by TSU on 2023-05-13.
//
#include "std.hpp"
#include "net/master_server.hpp"

hl::master::master_server::master_server()
        : abstract_server()
        , _accessor()
        , _accessor_count()
        , _accessor_robin()
        , _mutex()
        , _sessions()
{
    const auto config = hl::yaml::load("master.yaml");
    const auto accessor_count = config["thread"]["db_accessor_count"].as<size_t>(1);
    for (size_t i = 0; i < accessor_count; i++)
    {
        _accessor.push_back(std::make_shared<hl::database::accessor>());
    }
    _accessor_count = accessor_count;
}

void hl::master::master_server::on_accept(hl::master::master_session *session)
{
    LOGI << "Accepted master session from [" << session->get_remote_endpoint() << "]";
}

hl::database::accessor &hl::master::master_server::accessor()
{
    return *_accessor[(_accessor_robin++) % _accessor_count];
}

static auto get_server_type_string(server_type type)
{
    if (type == server_type::login)
        return "login";
    else
        return "game";
}

void hl::master::master_server::add(master_session& session)
{
    synchronized (_mutex)
    {
        auto servers = _sessions.find(session.get_server_type());
        if (servers == _sessions.end())
        {
            auto result = _sessions.emplace(session.get_server_type(), decltype(_sessions)::mapped_type());
            if (!result.second) // result.second -> emplace success or fail
                throw std::runtime_error("???");
            servers = result.first;
        }

        auto& sessions = servers->second;
        auto ret = sessions.find(session.get_idx());
        if (ret != sessions.end())
        {
            LOGW << "[" << get_server_type_string(session.get_server_type()) << "-" << session.get_idx() << "] is already registered";
            return;
        }

        std::shared_ptr<master_session> ms;
        if (!session.get_server().try_get(session.get_socket_sn(), ms))
        {
            LOGW << "[" << get_server_type_string(session.get_server_type()) << "-" << session.get_idx() << "] is not connected on server?";
            return;
        }
        sessions.emplace(session.get_idx(), ms);

        LOGI << "Registered " << "[" << get_server_type_string(session.get_server_type()) << "-" << session.get_idx() << "].";
        if (session.get_server_type() == server_type::game)
        {
            LOGI << "flag: " << session.get_game_flag() << " / endpoint: " << session.get_endpoint_address() << ":" << session.get_endpoint_port();
        }
    }
}

void hl::master::master_server::remove(master_session& session)
{
    synchronized (_mutex)
    {
        auto servers = _sessions.find(session.get_server_type());
        if (servers == _sessions.end())
            return;

        auto& sessions = servers->second;
        sessions.erase(session.get_idx());
    }
}

bool hl::master::master_server::try_get(server_type type, uint32_t id, std::shared_ptr<master_session> &ptr) const
{
    synchronized (_mutex)
    {
        auto servers = _sessions.find(type);
        if (servers == _sessions.end())
            return false;

        auto& sessions = servers->second;
        auto ret = sessions.find(id);
        if (ret == sessions.end())
            return false;

        ptr = ret->second;
    }
    return true;
}

auto hl::master::master_server::get(server_type type, uint32_t id) const -> std::shared_ptr<master_session>
{
    std::shared_ptr<master_session> ret;
    try_get(type, id, ret);
    return ret;
}
