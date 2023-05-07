//
// Created by TSU on 2023-05-07.
//

#include "std_common.hpp"
#include "net/abstract_server.hpp"

template<homeless::SessionType SessTy, homeless::AcceptorType<SessTy> AcceptTy>
homeless::abstract_server<SessTy, AcceptTy>::abstract_server(uv_loop_t* loop, std::string bind_address, uint16_t bind_port)
        : _sessions()
        , _acceptor(loop, std::move(bind_address), bind_port)
        , _session_pool()
        , _mutex()
        , _allocator()
{
    _sessions = _allocator.allocate(MaxSession);
    for (auto i = 0u; i < MaxSession; ++i)
        _session_pool.push(i);
}

template<homeless::SessionType SessTy, homeless::AcceptorType<SessTy> AcceptTy>
homeless::abstract_server<SessTy, AcceptTy>::~abstract_server()
{
    if (_session_pool.size() != MaxSession)
    {
        LOGF << "Not all sessions were released properly - this is a critical shutdown logic error!";
    }
    _allocator.deallocate(_sessions, MaxSession);
}

template<homeless::SessionType SessTy, homeless::AcceptorType<SessTy> AcceptTy>
size_t homeless::abstract_server<SessTy, AcceptTy>::get_connections() const
{
    synchronized (_mutex)
    {
        return MaxSession - _session_pool.size();
    }
}

template<homeless::SessionType SessTy, homeless::AcceptorType<SessTy> AcceptTy>
SessTy *homeless::abstract_server<SessTy, AcceptTy>::acquire_session()
{
    synchronized (_mutex)
    {
        if (_session_pool.empty())
            return nullptr;
        auto i = _session_pool.front();
        _session_pool.pop();
        auto session = &_sessions[i];
        std::allocator_traits<decltype(_allocator)>::construct(_allocator, session, i);
        LOGV << "acquired a login session - " << session->get_id();
        return session;
    }
}

template<homeless::SessionType SessTy, homeless::AcceptorType<SessTy> AcceptTy>
void homeless::abstract_server<SessTy, AcceptTy>::release_session(SessTy *session)
{
    synchronized (_mutex)
    {
        std::allocator_traits<decltype(_allocator)>::destroy(_allocator, session);
        _session_pool.push(session->get_id());
        LOGV << "released a login session - " << session->get_id();
    }
}