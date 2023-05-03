//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP
#define STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP

#include "io/in_buffer.hpp"
#include "io/out_buffer.hpp"
#include "memory/memory_pool.hpp"

class acceptor;

class abstract_session {
public:
    abstract_session();
    virtual ~abstract_session();

    abstract_session(const abstract_session&) = delete;
    abstract_session(abstract_session&&) = delete;
    abstract_session& operator= (const abstract_session&) = delete;
    abstract_session& operator= (abstract_session&&) = delete;

private:
    typedef memory_pool<SessionBufferSize, MaxSession> session_memory_pool;

    uv_tcp_t _client;
    bool _active;
    std::string _remote_address;
    std::string _remote_endpoint;
    session_memory_pool::memory_block* _read_buffer;

    void init_remote_address();

    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    void on_read(uv_stream_t* client, ssize_t read_size, const uv_buf_t* buf);
    void on_close(uv_handle_t* handle);

    friend class acceptor;

public:
    [[nodiscard]] std::string get_remote_address() const;
    [[nodiscard]] std::string get_remote_endpoint() const;

    void close();

    virtual void on_close(ssize_t reason) = 0;
    virtual void on_read(in_buffer& in_buf) = 0;
    void write(const out_buffer& out_buf);
};

#endif //STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP
