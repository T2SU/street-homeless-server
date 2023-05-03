//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_ACCEPTOR_HPP
#define STREET_HOMELESS_SERVER_ACCEPTOR_HPP

class acceptor
{
private:
    uv_loop_t* _loop;
    const std::string _bind_address;
    const uint16_t _bind_port;
    uv_tcp_t _server;
    struct sockaddr_in _addr;

public:
    acceptor(uv_loop_t* loop, std::string bind_address, uint16_t bind_port);
    virtual ~acceptor();

    acceptor(const acceptor&) = delete;
    acceptor& operator=(const acceptor&) = delete;
    acceptor(acceptor&&) = delete;
    acceptor& operator= (acceptor&&) = delete;

    void listen();
};

#endif //STREET_HOMELESS_SERVER_ACCEPTOR_HPP
