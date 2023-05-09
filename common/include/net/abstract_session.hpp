//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP
#define STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP

#include "io/in_buffer.hpp"
#include "io/out_buffer.hpp"
#include "memory/memory_pool.hpp"
#include "socket_job.hpp"

namespace hl
{
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
        std::atomic_bool _active;
        bool _closed;
        close_reason _closed_reason;
        std::string _remote_address;
        std::string _remote_endpoint;
        read_buffer _read_buffer;
        std::list<std::shared_ptr<write_job>> _write_jobs;
        const uint32_t _id;
        std::queue<socket_job> _jobs;
        std::mutex _mutex;
        std::atomic_bool _already_queued;

        void init_remote_address();

        void do_socket_op();
        void on_write(const std::shared_ptr<out_buffer>& out_buf);
        void enqueue_into_thread_pool(socket_job&& val);

        static void alloc_buffer_uv(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
        static void on_read_uv(uv_stream_t* client, ssize_t read_size, const uv_buf_t* buf);
        static void on_write_uv(uv_write_t *req, int status);

        friend class socket_thread;

    protected:
        virtual void on_packet(in_buffer& in_buffer) = 0;
        virtual void on_close(close_reason reason) = 0;

    public:
        [[nodiscard]] std::string get_remote_address() const;
        [[nodiscard]] std::string get_remote_endpoint() const;
        [[nodiscard]] uint32_t get_id() const;

        void close(close_reason reason = close_reason::server_close);
        void write(const out_buffer& out_buf);

        virtual const char* get_type_name() const = 0;

        template<typename Serv, typename SessTy> requires std::is_convertible_v<SessTy*, abstract_session*>
        void init(uv_loop_t* loop, uv_stream_t* server, Serv* serv, SessTy*)
        {
            auto stream = reinterpret_cast<uv_stream_t*>(&_client);
            uv_tcp_init(loop, &_client);
            auto accept_res = uv_accept(server, stream);
            if (accept_res)
            {
                LOGD << "failed to accept. close client force. reason:" << uv_strerror(accept_res);
                uv_close(reinterpret_cast<uv_handle_t*>(&_client), [](uv_handle_t*){});
                return;
            }
            _client.data = this;
            init_remote_address();
            serv->on_accept(reinterpret_cast<SessTy*>(this));
            uv_read_start(stream, alloc_buffer_uv, on_read_uv);
        }
    };

    std::ostream& operator<<(std::ostream& os, const hl::abstract_session& sess);
    std::ostream& operator<<(std::ostream& os, const hl::abstract_session* sess);
}


#endif //STREET_HOMELESS_SERVER_ABSTRACT_SESSION_HPP
