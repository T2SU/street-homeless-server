//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_LOGIN_SESSION_HPP
#define STREET_HOMELESS_SERVER_LOGIN_SESSION_HPP

#include <net/abstract_session.hpp>

namespace hl::login
{
    class login_session : public abstract_session
    {
    private:

    public:
        explicit login_session(uint32_t id);

        void on_packet(in_buffer& in_buffer) override;
        void on_close(close_reason reason) override;
        const char* get_type_name() const override;

    private:
        void on_check_alive_res(in_buffer& in_buf);
        void on_login_req(in_buffer& in_buf);
    };
}


#endif //STREET_HOMELESS_SERVER_LOGIN_SESSION_HPP
