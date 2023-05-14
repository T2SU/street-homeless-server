//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/game_server.hpp"

hl::game::game_server::game_server()
    : abstract_server()
    , _endpoint_address()
    , _endpoint_port()
    , _idx()
    , _flag()
{
}

void hl::game::game_server::on_accept(hl::game::game_session *session)
{
    LOGI << "Accepted game session from [" << session->get_remote_endpoint() << "]";
}

void hl::game::game_server::set_endpoint(std::string address, uint16_t port)
{
    _endpoint_address = std::move(address);
    _endpoint_port = port;
}

void hl::game::game_server::set_server_config(uint32_t idx, game_server_flag flag)
{
    _idx = idx;
    _flag = flag;
}

void hl::game::game_server::encode_config(out_buffer &obuf)
{
    obuf.write<uint8_t>(_idx);
    obuf.write(_flag);
    obuf.write_str(_endpoint_address);
    obuf.write(_endpoint_port);
}