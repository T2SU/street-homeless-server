//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP
#define STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP

#include "thread/socket_thread.hpp"
#include "net/abstract_session.hpp"
#include "net/acceptor.hpp"

template<typename SessTy>
concept SessionType = std::is_base_of_v<abstract_session, SessTy> && !std::is_same_v<abstract_session, SessTy>;

template<typename SessTy, typename AcceptTy>
concept AcceptorType = std::is_base_of_v<acceptor<SessTy>, AcceptTy> && !std::is_same_v<acceptor<SessTy>, AcceptTy>;

template<SessionType SessTy, AcceptorType<SessTy> AcceptTy>
class abstract_server
{
private:
    SessTy* _sessions;
    AcceptTy _acceptor;
    std::queue<size_t> _session_pool;
    mutable std::mutex _mutex;
    std::allocator<SessTy> _allocator;
    std::vector<std::shared_ptr<socket_thread>> _threads;

public:
    abstract_server(uv_loop_t* loop, std::string bind_address, uint16_t bind_port, uint32_t threads);
    virtual ~abstract_server();

    size_t get_connections() const;
    SessTy* acquire_session();
    void release_session(SessTy* session);

    void begin();
    void end();
};

#endif //STREET_HOMELESS_SERVER_ABSTRACT_SERVER_HPP
