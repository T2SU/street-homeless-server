//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_CONNECTOR_HPP
#define STREET_HOMELESS_SERVER_CONNECTOR_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "abstract_session.hpp"

namespace hl
{
    template<SessionType SessTy>
    class connector
    {
    private:
        uv_tcp_t _tcp;
        struct sockaddr_in _addr;
        uv_connect_t _con;
        const std::string& _target;
        const uint16_t _port;

        void on_connect(uv_connect_t* req)
        {
            auto session = std::make_shared<SessTy>(nullptr, 0, 0);
            session.init_connect(req);
            singleton<std::unique_lock<SessTy>>::get() = session;
            LOGI << "Connected to [" << _target << ":" << _port << "]. ";
        }

    public:
        connector(const std::string& target, uint16_t port)
                : _tcp() , _addr() , _con(), _target(target), _port(port)
        {
            uv_tcp_init(uv_default_loop(), &_tcp);
        }

        void connect()
        {
            uv_ip4_addr(_target.c_str(), _port, &_addr);
            _con.data = this;
            const auto res = uv_tcp_connect(&_con, &_tcp,
                                            reinterpret_cast<const struct sockaddr*>(&_addr),
                                            static_cast<uv_connect_cb>([](uv_connect_t *req, int status) {
                                                auto $this = reinterpret_cast<connector*>(req->data);
                                                if (status != 0)
                                                {
                                                    LOGE << "Failed to connect to [" << $this->_target << ":" << $this->_port << "]. " << uv_strerror(status);
                                                    exit(EXIT_FAILURE);
                                                }
                                                $this->on_connect(req);
                                            }));
            if (res != 0)
            {
                LOGE << "Failed to connect to [" << _target << ":" << _port << "]. " << uv_strerror(res);
                exit(EXIT_FAILURE);
            }
        }
    };
}

#endif //STREET_HOMELESS_SERVER_CONNECTOR_HPP
