//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP
#define STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP

#include "net/abstract_session.hpp"

namespace hl
{
    template<typename SessTy>
    concept SessionType = std::is_base_of_v<abstract_session, SessTy> && !std::is_same_v<abstract_session, SessTy>;

    template<SessionType SessTy>
    class abstract_server
    {
    private:
        uv_loop_t* _loop;
        uv_tcp_t _server;
        struct sockaddr_in _addr;

        SessTy* _sessions;
        std::queue<uint32_t> _session_pool;
        std::unordered_map<uint32_t, SessTy*> _connections;
        mutable std::mutex _mutex;
        std::allocator<SessTy> _allocator;

        static void on_accept_uv(uv_stream_t *server, int status);

        SessTy* acquire_session();

    public:
        abstract_server();
        virtual ~abstract_server();

        size_t get_connection_num() const;
        void broadcast(const out_buffer& out_buf);

        void release_session(SessTy* session);

        void begin(std::string bind_address, uint16_t bind_port);
        void end();

        virtual void on_accept(SessTy* session) = 0;
    };

    template<SessionType SessTy>
    abstract_server<SessTy>::abstract_server()
            : _loop(uv_default_loop())
            , _server()
            , _addr()
            , _sessions()
            , _session_pool()
            , _connections()
            , _mutex()
            , _allocator()
    {
        uv_tcp_init(_loop, &_server);
        _sessions = _allocator.allocate(MaxSession);
        for (uint32_t i = 0; i < MaxSession; ++i)
            _session_pool.push(i);
    }

    template<SessionType SessTy>
    abstract_server<SessTy>::~abstract_server()
    {
        if (_session_pool.size() != MaxSession)
        {
            LOGF << "Not all sessions were released properly - this is a critical shutdown logic error!";
        }
        _allocator.deallocate(_sessions, MaxSession);
    }

    template<SessionType SessTy>
    size_t abstract_server<SessTy>::get_connection_num() const
    {
        synchronized (_mutex)
        {
            return MaxSession - _session_pool.size();
        }
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::broadcast(const out_buffer &out_buf)
    {
        synchronized (_mutex)
        {
            for (auto& con : _connections)
            {
                con.second->write(out_buf);
            }
        }
    }

    template<SessionType SessTy>
    SessTy *abstract_server<SessTy>::acquire_session()
    {
        SessTy* session;
        uint32_t i;
        synchronized (_mutex)
        {
            if (_session_pool.empty())
                return nullptr;
            i = _session_pool.front();
            _session_pool.pop();
            session = &_sessions[i];
            _connections[i] = session;
        }
        std::allocator_traits<decltype(_allocator)>::construct(_allocator, session, i);
        LOGV << "acquired a session - " << session;
        return session;
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::release_session(SessTy *session)
    {
        LOGV << "releasing a session... - " << session;
        std::allocator_traits<decltype(_allocator)>::destroy(_allocator, session);
        synchronized (_mutex)
        {
            _session_pool.push(session->get_id());
            _connections.erase(session->get_id());
        }
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::begin(std::string bind_address, uint16_t bind_port)
    {
        uv_ip4_addr(bind_address.c_str(), bind_port, &_addr);
        _server.data = this;
        const auto bind_res = uv_tcp_bind(&_server, reinterpret_cast<const struct sockaddr*>(&_addr), 0);
        if (bind_res)
            throw acceptor_exception(std::format("bind ({})", uv_strerror(bind_res)), bind_address, bind_port);
        const auto listen_res = uv_listen(reinterpret_cast<uv_stream_t*>(&_server), DefaultBacklog, &on_accept_uv);
        if (listen_res)
            throw acceptor_exception(std::format("listen ({})", uv_strerror(listen_res)), bind_address, bind_port);
        LOGI << "Listening on " << bind_address << ":" << bind_port;
        uv_run(_loop, UV_RUN_DEFAULT);
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::end()
    {
        LOGI << "Stopping event loop..";
        uv_stop(_loop);
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::on_accept_uv(uv_stream_t *server, int status)
    {
        auto $this = reinterpret_cast<abstract_server<SessTy>*>(server->data);
        auto session = $this->acquire_session();
        session->init($this->_loop, server, $this, session);
    }
}
#endif //STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP
