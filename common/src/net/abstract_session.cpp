//
// Created by TSU on 2023-05-03.
//

#include "std_common.hpp"
#include "net/abstract_session.hpp"
#include "net/acceptor.hpp"
#include "utils/singleton.hpp"

abstract_session::abstract_session()
    : _client({})
    , _active()
    , _remote_address("?.?.?.?")
    , _remote_endpoint("?.?.?.?:0")
    , _read_buffer(singleton<session_memory_pool>::get().acquire())
{
}

abstract_session::~abstract_session()
{
}

void abstract_session::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{

}

void abstract_session::on_read(uv_stream_t *client, ssize_t read_size, const uv_buf_t *buf)
{

}

void abstract_session::on_close(uv_handle_t *handle)
{

}

std::string abstract_session::get_remote_address() const
{
    return _remote_address;
}

std::string abstract_session::get_remote_endpoint() const
{
    return _remote_endpoint;
}

void abstract_session::init_remote_address()
{
    char result[50];
    struct sockaddr_storage addr{};
    int addr_len = 0;
    auto port = 0;
    if (uv_tcp_getpeername(&_client, reinterpret_cast<struct sockaddr*>(&addr), &addr_len))
    {
        _remote_address = "?.?.?.?";
        return;
    }
    uv_ip_name(reinterpret_cast<struct sockaddr*>(&addr), result, sizeof(result));
    if (addr.ss_family == AF_INET) port = reinterpret_cast<struct sockaddr_in*>(&addr)->sin_port;
    if (addr.ss_family == AF_INET6) port = reinterpret_cast<struct sockaddr_in6*>(&addr)->sin6_port;
    _remote_address = result;
    _remote_endpoint = std::format("{}:{}", result, port);
}