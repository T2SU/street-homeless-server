//
// Created by TSU on 2023-05-03.
//

#include "std_common.hpp"
#include "net/acceptor.hpp"

acceptor::acceptor(uv_loop_t *loop, std::string bind_address, uint16_t bind_port)
    : _loop(loop)
    , _bind_address(std::move(bind_address))
    , _bind_port(bind_port)
    , _server()
    , _addr()
{
    uv_tcp_init(_loop, &_server);
    LOGV << "Created an acceptor (" << _bind_address << ":" << _bind_port << ")";
}

acceptor::~acceptor()
{
    LOGV << "Destroyed an acceptor (" << _bind_address << ":" << _bind_port << ")";
}

void acceptor::listen()
{
    uv_ip4_addr(_bind_address.c_str(), _bind_port, &_addr);
    auto bind_res = uv_tcp_bind(&_server, reinterpret_cast<const struct sockaddr*>(&_addr), 0);
    if (bind_res)
    {
        
    }
}
