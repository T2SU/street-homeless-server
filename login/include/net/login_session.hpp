//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_LOGIN_SESSION_HPP
#define STREET_HOMELESS_SERVER_LOGIN_SESSION_HPP

#include <net/abstract_session.hpp>

class login_session : public abstract_session
{
private:

public:
    explicit login_session(uint32_t id);

    void on_packet(const in_buffer& in_buffer) override;
    void on_close(close_reason reason) override;
};


#endif //STREET_HOMELESS_SERVER_LOGIN_SESSION_HPP
