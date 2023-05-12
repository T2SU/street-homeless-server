//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP
#define STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "abstract_session.hpp"
#include "server.hpp"
#include "utils/yaml_config.hpp"

namespace hl
{
    template<typename SessTy>
    concept SessionType = std::is_base_of_v<abstract_session, SessTy> && !std::is_same_v<abstract_session, SessTy>;

    template<SessionType SessTy>
    class abstract_server : public server
    {
    private:
        uv_loop_t* _loop;
        uv_tcp_t _server;
        struct sockaddr_in _addr;

        SessTy* _sessions;
        std::queue<uint32_t> _session_pool;
        std::unordered_map<uint32_t, std::shared_ptr<SessTy>> _connected_sessions;
        mutable std::recursive_mutex _mutex;
        std::allocator<SessTy> _allocator;
        std::atomic_uint32_t _session_sn_counter;

        int32_t _ping_interval;
        int32_t _ping_timeout;
        int32_t _packet_error_threshold;

        static void on_accept_uv(uv_stream_t *server, int status);

        SessTy* acquire_session();

    public:
        abstract_server();
        virtual ~abstract_server();

        size_t get_connection_num() const;
        void broadcast(const out_buffer& out_buf);

        void remove_from_connected(abstract_session* session) override;
        void release_session(abstract_session* session) override;
        bool try_get(uint32_t socket_sn, std::shared_ptr<SessTy>& ptr);

        void begin(const std::string& bind_address, uint16_t bind_port);
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
            , _connected_sessions()
            , _mutex()
            , _allocator()
            , _session_sn_counter()
            , _ping_interval()
            , _ping_timeout()
            , _packet_error_threshold()
    {
        uv_tcp_init(_loop, &_server);
        _sessions = _allocator.allocate(MaxSession);
        for (uint32_t i = 0; i < MaxSession; ++i)
            _session_pool.push(i);

        const auto node = hl::yaml::load("common.yaml");
        _ping_interval = node["network"]["ping"]["interval"].as<int32_t>(30);
        _ping_timeout = node["network"]["ping"]["timeout"].as<int32_t>(60);
        _packet_error_threshold = node["security"]["session"]["packet_error_threshold"].as<int32_t>(32);
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
        std::vector<typename decltype(_connected_sessions)::value_type> targets;

        synchronized (_mutex)
        {
            std::copy(_connected_sessions.begin(), _connected_sessions.end(), targets.end());
        }
        for (auto& con : targets)
        {
            con.second->write(out_buf);
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
            std::allocator_traits<decltype(_allocator)>::construct(_allocator, session, this, i, ++_session_sn_counter);
            _connected_sessions[session->get_socket_sn()] = std::shared_ptr<SessTy>(session, [this](SessTy* session) {
                release_session(session);
            });
        }
        LOGV << "acquired a session - " << session;
        return session;
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::remove_from_connected(abstract_session *session)
    {
        LOGV << "remove from connected sessions - " << session;
        synchronized (_mutex)
        {
            _connected_sessions.erase(session->get_socket_sn());
        }
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::release_session(abstract_session *session)
    {
        LOGV << "releasing a session... - " << session;
        std::allocator_traits<decltype(_allocator)>::destroy(_allocator, session);
        synchronized (_mutex)
        {
            _session_pool.push(session->get_id());
        }
    }

    template<SessionType SessTy>
    bool abstract_server<SessTy>::try_get(uint32_t socket_sn, std::shared_ptr<SessTy> &ptr)
    {
        synchronized (_mutex)
        {
            auto found = _connected_sessions.find(socket_sn);
            if (found != _connected_sessions.end())
            {
                ptr = found->second;
                return true;
            }
        }
        return false;
    }

    template<SessionType SessTy>
    void abstract_server<SessTy>::begin(const std::string& bind_address, uint16_t bind_port)
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
