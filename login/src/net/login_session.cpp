//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_session.hpp"
#include "net/login_server.hpp"

hl::login::login_session::login_session(uint32_t id)
        : abstract_session(id)
{}

void hl::login::login_session::on_close(close_reason reason)
{
    LOGI << this << "on close login session.";
    hl::singleton<login_server>::get().release_session(this);
}

const char *hl::login::login_session::get_type_name() const
{
    return "login_session";
}

void hl::login::login_session::on_packet(in_buffer &in_buffer)
{
    LOGI << this << "on packet login session.";
    const auto op = in_buffer.read<cli_msg>();
    switch (op)
    {
        case cli_msg::CheckAliveRes:
            on_check_alive_res(in_buffer);
            break;
        case cli_msg::LoginReq:
            on_login_req(in_buffer);
            break;
    }
}

void hl::login::login_session::on_check_alive_res(in_buffer &in_buf)
{

}

void hl::login::login_session::on_login_req(in_buffer &in_buf)
{
    const auto username(in_buf.read_str());
    const auto password(in_buf.read_str());

    LOGI << "username = " << username;
    LOGI << "password = " << password;

    out_buffer out_buf(svr_msg::LoginRes);
    if (username == "test" && password == "test")
    {
        out_buf.write<char>(0);
        out_buf.write_str("가난 시뮬레이션에 오신 것을 환영합니다!\nWelcome to the homeless simulator!!");
    }
    else
    {
        out_buf.write<char>(1);
    }
    write(out_buf);
}
