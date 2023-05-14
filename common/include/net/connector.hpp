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
        struct sockaddr_in _addr;
        uv_connect_t _con;
        std::string _target;
        uint16_t _port;
        uv_tcp_t* _handle;

        void on_connect(uv_connect_t* req)
        {
            singleton<std::unique_ptr<SessTy>>::get()->init_connect();
            LOGI << "Connected to [" << _target << ":" << _port << "]. ";
        }

    public:
        connector()
                : _addr() , _con(), _target(), _port()
        {
            auto session = std::make_unique<SessTy>(nullptr, 0, 0);
            singleton<std::unique_ptr<SessTy>>::get() = std::move(session);
            _handle = session->get_handle();
            uv_tcp_init(uv_default_loop(), _handle);
        }

        void connect(const std::string& target, uint16_t port)
        {
            using std::chrono_literals::operator""s;
            _target = target;
            _port = port;
            uv_ip4_addr(_target.c_str(), _port, &_addr);
            _con.data = this;
            const auto res = uv_tcp_connect(&_con, _handle,
                                            reinterpret_cast<const struct sockaddr*>(&_addr),
                                            static_cast<uv_connect_cb>([](uv_connect_t *req, int status) {
                                                auto $this = reinterpret_cast<connector*>(req->data);
                                                if (status != 0)
                                                {
                                                    LOGE << "Failed to connect to [" << $this->_target << ":" << $this->_port << "]. " << uv_strerror(status);
                                                    std::this_thread::sleep_for(2s);
                                                    $this->connect($this->_target, $this->_port);
                                                    return;
                                                }
                                                $this->on_connect(req);
                                            }));
            if (res != 0)
            {
                LOGE << "Failed to connect to [" << _target << ":" << _port << "]. " << uv_strerror(res);
                std::this_thread::sleep_for(2s);
                connect(target, port);
            }
        }
    };
}

#endif //STREET_HOMELESS_SERVER_CONNECTOR_HPP
