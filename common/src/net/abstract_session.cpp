//
// Created by TSU on 2023-05-03.
//

#include "std_common.hpp"
#include "net/abstract_session.hpp"
#include "net/acceptor.hpp"
#include "net/packet.hpp"
#include "utils/singleton.hpp"

abstract_session::abstract_session(uint32_t id)
    : _client({})
    , _active()
    , _remote_address("?.?.?.?")
    , _remote_endpoint("?.?.?.?:0")
    , _read_buffer()
    , _id(id)
{
    _read_buffer.mode = read_mode::HEADER;
    LOGV << "Constructed session (" << __FUNCTION__ << ")";
}

abstract_session::~abstract_session()
{
    LOGV << "Destructed session (" << __FUNCTION__ << ")";
}

void abstract_session::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    auto $this = reinterpret_cast<abstract_session*>(handle->data);
    auto& rb = $this->_read_buffer;
    buf->base = &rb.buffer[rb.offset];
    buf->len = sizeof(rb.buffer) - rb.offset;
}

void abstract_session::on_read(uv_stream_t *client, ssize_t read_size, const uv_buf_t *buf)
{
    auto $this = reinterpret_cast<abstract_session*>(client->data);
    auto& rb = $this->_read_buffer;
    packet_header ph;
    while (read_size > 0)
    {
        switch (rb.mode)
        {
            case read_mode::HEADER:
            {
                if (read_size < sizeof(packet_header))
                {
                    LOGV << "[HEADER] read_size = " << read_size << " (" << read_size << " < " << sizeof(packet_header) << ")";
                    return;
                }
                read_size -= sizeof(packet_header);
                std::memcpy(&ph, &rb.buffer[rb.offset], sizeof(packet_header));
                rb.offset += sizeof(packet_header);

                // TODO: decrypt header checksum and length (and validation)

                rb.mode = read_mode::CONTENT;
                break;
            }
            case read_mode::CONTENT:
            {
                if (read_size < ph.length)
                {
                    LOGV << "[CONTENT] read_size = " << read_size << " (" << read_size << " < " << packet.header.length << ")";
                    return;
                }
                read_size -= ph.length;
                // TODO: decrypt packet content
                // decrypt(&rb.buffer[rb.offset], ph.length)
                auto in_buf = std::make_shared<in_buffer>(&rb.buffer[rb.offset], ph.length);
                rb.offset += ph.length;

                // add in_buffer
                // notify socket thread

                break;
            }
        }
    }
}

void abstract_session::on_close(uv_handle_t *handle)
{
    auto $this = reinterpret_cast<abstract_session*>(handle->data);
}

std::string abstract_session::get_remote_address() const
{
    return _remote_address;
}

std::string abstract_session::get_remote_endpoint() const
{
    return _remote_endpoint;
}

uint32_t abstract_session::get_id() const
{
    return _id;
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

void abstract_session::close()
{

}

void abstract_session::write(const out_buffer &out_buf)
{

}
