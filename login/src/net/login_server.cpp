//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_server.hpp"

void hl::login::login_server::on_accept(hl::login::login_session *session)
{
    LOGI << "Accepted login session from [" << session->get_remote_endpoint() << "]";
}

hl::database::accessor &hl::login::login_server::accessor()
{
    return *_accessor[(_accessor_robin++) % _accessor_count];
}

hl::login::login_server::login_server()
        : abstract_server()
        , _accessor()
        , _accessor_count()
        , _accessor_robin()
        , _idx()
{
    const auto config = hl::yaml::load("login.yaml");
    const auto accessor_count = config["thread"]["db_accessor_count"].as<size_t>(1);
    for (size_t i = 0; i < accessor_count; i++)
    {
        _accessor.push_back(std::make_shared<hl::database::accessor>());
    }
    _accessor_count = accessor_count;
    _idx = config["config"]["login"]["index"].as<int32_t>();
}

void hl::login::login_server::encode_config(out_buffer &obuf)
{
    obuf.write(server_type::login);
    obuf.write<uint8_t>(_idx);
}