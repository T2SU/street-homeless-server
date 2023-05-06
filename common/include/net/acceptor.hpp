//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_ACCEPTOR_HPP
#define STREET_HOMELESS_SERVER_ACCEPTOR_HPP

#include "abstract_session.hpp"

template<typename SessionTy> requires (std::is_base_of_v<abstract_session, SessionTy> && !std::is_same_v<abstract_session, SessionTy>)
class acceptor
{
private:
    uv_loop_t* _loop;
    const std::string _bind_address;
    const uint16_t _bind_port;
    uv_tcp_t _server;
    struct sockaddr_in _addr;

public:
    acceptor(uv_loop_t *loop, std::string bind_address, uint16_t bind_port)
            : _loop(loop)
            , _bind_address(std::move(bind_address))
            , _bind_port(bind_port)
            , _server()
            , _addr()
    {
        uv_tcp_init(_loop, &_server);
        LOGV << "Created an acceptor (" << _bind_address << ":" << _bind_port << ")";
    }

    virtual ~acceptor()
    {
        LOGV << "Destroyed an acceptor (" << _bind_address << ":" << _bind_port << ")";
    }

    acceptor(const acceptor&) = delete;
    acceptor& operator=(const acceptor&) = delete;
    acceptor(acceptor&&) = delete;
    acceptor& operator= (acceptor&&) = delete;

    void listen()
    {
        uv_ip4_addr(_bind_address.c_str(), _bind_port, &_addr);
        _server.data = this;
        const auto bind_res = uv_tcp_bind(&_server, reinterpret_cast<const struct sockaddr*>(&_addr), 0);
        if (bind_res)
            throw acceptor_exception(std::format("bind ({})", uv_strerror(bind_res)), _bind_address, _bind_port);
        const auto listen_res = uv_listen(reinterpret_cast<uv_stream_t*>(&_server), DefaultBacklog, &on_new_connection);
        if (listen_res)
            throw acceptor_exception(std::format("listen ({})", uv_strerror(listen_res)), _bind_address, _bind_port);
        LOGI << "Listening on " << _bind_address << ":" << _bind_port;
    }

private:
    static void on_new_connection(uv_stream_t *server, int status)
    {
        auto $this = reinterpret_cast<acceptor*>(server->data);
        auto session = $this->create_session();
        uv_tcp_init($this->_loop, &session->_client);
        if (!uv_accept($this->_server, reinterpret_cast<uv_stream_t*>(&session->_client)))
        {
            uv_close(reinterpret_cast<uv_handle_t*>(&session->_client), SessionTy::on_close);
            return;
        }
        session->_client.data = session;
        session->init_remote_address();
        $this->on_accept(session);
        uv_read_start(reinterpret_cast<uv_stream_t*>(&session->_client), SessionTy::alloc_buffer, SessionTy::on_read);
    }

protected:
    virtual SessionTy* create_session() = 0;
    virtual void on_accept(SessionTy* session) = 0;
};

#endif //STREET_HOMELESS_SERVER_ACCEPTOR_HPP
