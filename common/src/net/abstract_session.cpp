//
// Created by TSU on 2023-05-03.
//

#include "std_common.hpp"
#include "net/abstract_session.hpp"
#include "net/packet.hpp"
#include "utils/singleton.hpp"
#include "thread/socket_thread_pool.hpp"

static const char* close_reason_to_str(close_reason reason);

hl::abstract_session::abstract_session(server* server, uint32_t id, uint32_t socket_sn)
    : _client({})
    , _active(true)
    , _closed(false)
    , _closed_reason(close_reason::unknown)
    , _remote_address("?.?.?.?")
    , _remote_endpoint("?.?.?.?:0")
    , _read_buffer()
    , _write_jobs(10)
    , _id(id)
    , _socket_sn(socket_sn)
    , _jobs()
    , _mutex()
    , _already_queued()
    , _packet_error_count()
    , _server(server)
{
    _read_buffer.mode = read_mode::HEADER;
    LOGV << "Constructed session (" << __FUNCTION__ << ")";
}

hl::abstract_session::~abstract_session()
{
    LOGV << "Destructed session (" << __FUNCTION__ << ")";
}

void hl::abstract_session::init_remote_address()
{
    char result[50];
    struct sockaddr_storage addr{};
    int addr_len = sizeof(addr);
    auto port = 0;
    auto peer_name_res = uv_tcp_getpeername(&_client, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);
    if (peer_name_res)
    {
        LOGD << this << "failed to 'getpeername'. reason: " << uv_strerror(peer_name_res);
        _remote_address = "?.?.?.?";
        return;
    }
    uv_ip_name(reinterpret_cast<struct sockaddr*>(&addr), result, sizeof(result));
    if (addr.ss_family == AF_INET) port = reinterpret_cast<struct sockaddr_in*>(&addr)->sin_port;
    if (addr.ss_family == AF_INET6) port = reinterpret_cast<struct sockaddr_in6*>(&addr)->sin6_port;
    _remote_address = result;
    _remote_endpoint = std::format("{}:{}", result, port);
}

std::string hl::abstract_session::get_remote_address() const
{
    return _remote_address;
}

std::string hl::abstract_session::get_remote_endpoint() const
{
    return _remote_endpoint;
}

uint32_t hl::abstract_session::get_id() const
{
    return _id;
}

uint32_t hl::abstract_session::get_socket_sn() const
{
    return _socket_sn;
}

bool hl::abstract_session::is_active() const
{
    return _active;
}

void hl::abstract_session::alloc_buffer_uv(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    auto $this = reinterpret_cast<abstract_session*>(handle->data);
    auto& rb = $this->_read_buffer;
    buf->base = &rb.buffer[rb.offset];
    buf->len = sizeof(rb.buffer) - rb.offset;
}

void hl::abstract_session::on_read_uv(uv_stream_t *client, ssize_t read_size, const uv_buf_t *buf)
{
    auto $this = reinterpret_cast<abstract_session*>(client->data);
    if (read_size < 0)
    {
        if (read_size != UV_EOF)
        {
            LOGD << $this << "read error - " << uv_err_name(static_cast<int>(read_size));
            $this->close(close_reason::exception);
        }
        else
        {
            $this->close(close_reason::gracefully_shutdown);
        }
        return;
    }
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
                    LOGV << $this << "[HEADER] read_size = " << read_size << " (" << read_size << " < " << sizeof(packet_header) << ")";
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
                    LOGV << $this << "[CONTENT] read_size = " << read_size << " (" << read_size << " < " << ph.length << ")";
                    return;
                }
                read_size -= ph.length;
                // TODO: decrypt packet content
                // decrypt(&rb.buffer[rb.offset], ph.length)
                auto in_buf = std::make_shared<in_buffer>(&rb.buffer[rb.offset], ph.length);
                rb.offset += ph.length;

                socket_job job{.op=socket_op::READ, .in_buffer=in_buf};
                $this->enqueue_into_thread_pool(std::move(job));

                rb.mode = read_mode::HEADER;
                break;
            }
        }
    }
}

void hl::abstract_session::on_write_uv(uv_write_t *req, int status)
{
    auto $this = reinterpret_cast<abstract_session*>(req->data);
    if (status)
    {
        LOGD << $this << "write callback error - " << uv_strerror(status);
        $this->close(close_reason::exception);
        return;
    }
    auto wj = reinterpret_cast<write_job*>(req);
    $this->_write_jobs.erase(wj->pointer);
}

void hl::abstract_session::do_socket_op()
{
    socket_job job;

    while (true)
    {
        synchronized (_mutex)
        {
            if (_jobs.empty())
                return;
            job = _jobs.front();
            _jobs.pop();
        }
        switch (job.op)
        {
            case socket_op::READ:
            {
                LOGV << this << "called on_packet";
                LOGD << this << "[R/" << get_remote_endpoint() << "] " << job.in_buffer->dump_packet();
                try
                {
                    on_packet(*(job.in_buffer));
                }
                catch (const std::exception& ex)
                {
                    LOGE << "error on handling packet from (" << get_remote_endpoint() << "). dump=[" << job.in_buffer->dump_packet() << "]";
                    if (++_packet_error_count >= 32)
                    {
                        LOGW << "too many packet error from (" << get_remote_endpoint() << "). disconnect session forcely.";
                        close();
                    }
                }
                break;
            }
            case socket_op::WRITE:
            {
                if (_active)
                {
                    LOGV << this << "called on_write";
                    LOGD << this << "[S/" << get_remote_endpoint() << "] " << job.out_buffer->dump_packet();
                    on_write(job.out_buffer);
                }
                else LOGV << this << "write event was fired, but not active.";
                break;
            }
            case socket_op::CLOSE:
            {
                if (!_closed)
                {
                    LOGV << this << "called uv_close";
                    uv_close(reinterpret_cast<uv_handle_t*>(&_client), nullptr);
                    _closed = true;
                    LOGV << this << "called on_close.  reason: " << close_reason_to_str(_closed_reason);
                    on_close(_closed_reason);
                    _server->remove_from_connected(this);
                }
                else LOGV << this << "close event was fired, but already closed.";
                break;
            }
        }
    }
}

void hl::abstract_session::on_write(const std::shared_ptr<out_buffer>& out_buf)
{
    std::shared_ptr<write_job> wj = std::make_shared<write_job>();
    wj->req.data = this;
    wj->out_buf = std::make_shared<out_buffer>();

    // TODO encrypt header
    packet_header ph{.checksum=0, .length=static_cast<uint16_t>(out_buf->get_buffer_size())};
    // TODO encrypt content

    wj->out_buf->write(ph);
    wj->out_buf->write(out_buf->get_buffer(), 0, out_buf->get_buffer_size());
    wj->buf = uv_buf_init((char*)wj->out_buf->get_buffer(), wj->out_buf->get_buffer_size());
    wj->pointer = _write_jobs.insert(_write_jobs.end(), wj);
    auto res = uv_write(reinterpret_cast<uv_write_t*>(&wj->pointer->get()->req), reinterpret_cast<uv_stream_t*>(&_client), &(wj->pointer->get()->buf), 1, on_write_uv);
    if (res)
    {
        LOGD << this << "write error - " << uv_strerror(res);
        close(close_reason::exception);
    }
}

void hl::abstract_session::write(const out_buffer &out_buf)
{
    if (!_active) return;
    socket_job job{.op=socket_op::WRITE, .out_buffer=std::make_shared<out_buffer>(out_buf)};
    enqueue_into_thread_pool(std::move(job));
    LOGV << this << "requested write packet. (size=" << out_buf.get_buffer_size() << ")";
}

void hl::abstract_session::close(close_reason reason)
{
    bool $true = true;
    if (!_active.compare_exchange_strong($true, false))
    {
        LOGD << this << "requested closing session (reason: " << close_reason_to_str(reason) << "), but already not active.";
        return;
    }
    _closed_reason = reason;
    socket_job job{.op=socket_op::CLOSE, .close_reason=reason};
    enqueue_into_thread_pool(std::move(job));
    LOGV << this << "requested closing session. reason: " << close_reason_to_str(reason);
}

void hl::abstract_session::enqueue_into_thread_pool(socket_job&& val)
{
    synchronized (_mutex)
    {
        _jobs.emplace(val);
    }
    singleton<socket_thread_pool>::get().get_worker_thread()->enqueue(this);
}

static const char* close_reason_to_str(close_reason reason)
{
    switch (reason)
    {
        case close_reason::exception:
            return "exception";
        case close_reason::server_close:
            return "server_close";
        case close_reason::connection_reset:
            return "connection_reset";
        case close_reason::gracefully_shutdown:
            return "gracefully_shutdown";
        default:
            return "unknown";
    }
}