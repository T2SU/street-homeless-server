//
// Created by TSU on 2023-05-03.
//

#include "std_common.hpp"
#include "net/abstract_session.hpp"
#include "net/packet.hpp"
#include "utils/singleton.hpp"
#include "thread/socket_thread_pool.hpp"

homeless::abstract_session::abstract_session(uint32_t id)
    : _client({})
    , _active(true)
    , _closed(false)
    , _closed_reason(close_reason::unknown)
    , _remote_address("?.?.?.?")
    , _remote_endpoint("?.?.?.?:0")
    , _read_buffer()
    , _write_jobs(10)
    , _id(id)
    , _jobs()
    , _mutex()
    , _already_queued()
{
    _read_buffer.mode = read_mode::HEADER;
    LOGV << "Constructed session (" << __FUNCTION__ << ")";
}

homeless::abstract_session::~abstract_session()
{
    LOGV << "Destructed session (" << __FUNCTION__ << ")";
}

void homeless::abstract_session::init_remote_address()
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

std::string homeless::abstract_session::get_remote_address() const
{
    return _remote_address;
}

std::string homeless::abstract_session::get_remote_endpoint() const
{
    return _remote_endpoint;
}

uint32_t homeless::abstract_session::get_id() const
{
    return _id;
}

void homeless::abstract_session::alloc_buffer_uv(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    auto $this = reinterpret_cast<abstract_session*>(handle->data);
    auto& rb = $this->_read_buffer;
    buf->base = &rb.buffer[rb.offset];
    buf->len = sizeof(rb.buffer) - rb.offset;
}

void homeless::abstract_session::on_read_uv(uv_stream_t *client, ssize_t read_size, const uv_buf_t *buf)
{
    auto $this = reinterpret_cast<abstract_session*>(client->data);
    if (read_size < 0)
    {
        if (read_size != UV_EOF)
        {
            LOGD << "[socket-" << $this->get_id() << "] read error - " << uv_err_name(static_cast<int>(read_size));
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
                    LOGV << "[CONTENT] read_size = " << read_size << " (" << read_size << " < " << ph.length << ")";
                    return;
                }
                read_size -= ph.length;
                // TODO: decrypt packet content
                // decrypt(&rb.buffer[rb.offset], ph.length)
                auto in_buf = std::make_shared<in_buffer>(&rb.buffer[rb.offset], ph.length);
                rb.offset += ph.length;

                socket_job job{.op=socket_op::READ, .in_buffer=in_buf};
                $this->enqueue_into_thread_pool(std::move(job));

                break;
            }
        }
    }
}

void homeless::abstract_session::on_write_uv(uv_write_t *req, int status)
{
    auto $this = reinterpret_cast<abstract_session*>(req->data);
    if (status)
    {
        LOGD << "[socket-" << $this->get_id() << "] write callback error - " << uv_strerror(status);
        $this->close(close_reason::exception);
        return;
    }
    auto erased = std::erase_if($this->_write_jobs, [req](write_job& job){ return req == &job.req; });
    if (erased)
    {
        LOGV << "[socket-" << $this->get_id() << "] successfully deleted wrote job.";
    }
    else
    {
        LOGV << "[socket-" << $this->get_id() << "] failed to delete wrote job. (" << std::hex << req << ")";
    }
}

void homeless::abstract_session::on_close_uv(uv_handle_t *handle)
{
    auto $this = reinterpret_cast<abstract_session*>(handle->data);
    LOGV << "[socket-" << $this->get_id() << "] called on_close.  reason: " << close_reason_to_str($this->_closed_reason);
    $this->on_close($this->_closed_reason);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
void homeless::abstract_session::do_socket_op()
{
    socket_job job;

    while (true)
    {
        synchronized (_mutex)
        {
            if (_jobs.empty())
                break;
            job = _jobs.front();
            _jobs.pop();
        }
        switch (job.op)
        {
            case socket_op::READ:
            {
                LOGV << "[socket-" << get_id() << "] called on_packet";
                on_packet(*(job.in_buffer));
                break;
            }
            case socket_op::WRITE:
            {
                if (_active)
                {
                    LOGV << "[socket-" << get_id() << "] called on_write";
                    on_write(job.out_buffer);
                }
                else LOGV << "[socket-" << get_id() << "] write event was fired, but not active.";
                break;
            }
            case socket_op::CLOSE:
            {
                if (!_closed)
                {
                    LOGV << "[socket-" << get_id() << "] called uv_close";
                    uv_close(reinterpret_cast<uv_handle_t*>(&_client), on_close_uv);
                    _closed = true;
                }
                else LOGV << "[socket-" << get_id() << "] close event was fired, but already closed.";
                break;
            }
        }
    }
}
#pragma clang diagnostic pop

void homeless::abstract_session::on_write(std::shared_ptr<out_buffer> out_buf)
{
    write_job wj;
    wj.req.data = this;
    wj.out_buf = out_buf;
    wj.buf = uv_buf_init((char*)out_buf->get_buffer(), out_buf->get_buffer_size());
    auto it = &_write_jobs.emplace_back(wj);
    auto res = uv_write(reinterpret_cast<uv_write_t*>(it), reinterpret_cast<uv_stream_t*>(&_client), &(it->buf), 1, on_write_uv);
    if (res)
    {
        LOGD << "[socket-" << get_id() << "] write error - " << uv_strerror(res);
        close(close_reason::exception);
    }
}

void homeless::abstract_session::write(const out_buffer &out_buf)
{
    if (!_active) return;
    socket_job job{.op=socket_op::WRITE, .out_buffer=std::make_shared<out_buffer>(out_buf)};
    enqueue_into_thread_pool(std::move(job));
    LOGV << "[socket-" << get_id() << "] requested write packet. (size=" << out_buf.get_buffer_size() << ")";
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

void homeless::abstract_session::close(close_reason reason)
{
    _active = false;
    socket_job job{.op=socket_op::CLOSE, .close_reason=reason};
    enqueue_into_thread_pool(std::move(job));
    LOGV << "[socket-" << get_id() << "] requested closing session. reason: " << close_reason_to_str(reason);
}

void homeless::abstract_session::enqueue_into_thread_pool(socket_job&& val)
{
    synchronized (_mutex)
    {
        _jobs.emplace(val);
    }
    singleton<socket_thread_pool>::get().get_worker_thread()->enqueue(this);
}
