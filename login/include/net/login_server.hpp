//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP
#define STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP

#include "login_session.hpp"
#include "login_acceptor.hpp"

class login_server
{
private:
    login_session* _sessions;
    login_acceptor _acceptor;
    std::queue<size_t> _session_pool;
    mutable std::mutex _mutex;
    std::allocator<login_session> _allocator;

public:
    login_server(uv_loop_t* loop, std::string bind_address, uint16_t bind_port);
    virtual ~login_server();

    size_t get_connections() const;
    login_session* acquire_session();
    void release_session(login_session* session);
};


#endif //STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP
