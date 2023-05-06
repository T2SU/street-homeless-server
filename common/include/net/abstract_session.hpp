//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP
#define STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP

#include "io/in_buffer.hpp"
#include "io/out_buffer.hpp"
#include "memory/memory_pool.hpp"

class abstract_session {
public:
    explicit abstract_session(uint32_t id);
    virtual ~abstract_session();

    abstract_session(const abstract_session&) = delete;
    abstract_session(abstract_session&&) = delete;
    abstract_session& operator= (const abstract_session&) = delete;
    abstract_session& operator= (abstract_session&&) = delete;

private:
    enum class read_mode { HEADER, CONTENT };
    typedef struct
    {
        read_mode mode;
        char buffer[SessionBufferSize];
        size_t offset;
    } read_buffer;

    uv_tcp_t _client;
    bool _active;
    std::string _remote_address;
    std::string _remote_endpoint;
    read_buffer _read_buffer;
    const uint32_t _id;

    void init_remote_address();

    static void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    static void on_read(uv_stream_t* client, ssize_t read_size, const uv_buf_t* buf);
    static void on_close(uv_handle_t* handle);

    template<typename SessionTy> requires (std::is_base_of_v<abstract_session, SessionTy> && !std::is_same_v<abstract_session, SessionTy>)
    friend class acceptor;
    friend class socket_thread;

protected:
    virtual void on_packet(const in_buffer& in_buffer) = 0;
    virtual void on_close(close_reason reason) = 0;

public:
    [[nodiscard]] std::string get_remote_address() const;
    [[nodiscard]] std::string get_remote_endpoint() const;
    [[nodiscard]] uint32_t get_id() const;

    void close();
    void write(const out_buffer& out_buf);
};

#endif //STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP
