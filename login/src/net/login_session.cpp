//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_session.hpp"
#include "net/login_server.hpp"
#include "account_data.pb.h"
#include "login_result.pb.h"

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
    const auto op = in_buffer.read<uint16_t>();
    switch (op)
    {
        case pb::ClientMessage::CheckAliveRes:
            on_check_alive_res(in_buffer);
            break;
        case pb::ClientMessage::LoginReq:
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

    out_buffer out_buf(pb::ServerMessage::LoginRes);
    if (username != "test")
        out_buf.write<char>(pb::LoginResult::InvalidAccount);
    else if (password != "test")
        out_buf.write<char>(pb::LoginResult::InvalidPassword);
    else
    {
        pb::AccountData account_data;
        out_buf.write(pb::LoginResult::Success);
        account_data.set_uid(1024);
        account_data.set_name(username);
        out_buf.write_pb(account_data);
    }
    write(out_buf);
}
