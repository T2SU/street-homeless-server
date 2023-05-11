//
// Created by TSU on 2023-05-11.
//


#include "std.hpp"
#include "handlers/login_req.hpp"

class login_job : public hl::database::job
{
private:
    const uint32_t _socket_sn;
    const std::string _username;
    const std::string _password;

public:
    login_job(uint32_t socket_sn, std::string username, std::string password)
        : _socket_sn(socket_sn), _username(std::move(username)), _password(std::move(password))
    {}

    void process(sqlpp::mysql::connection& conn) override
    {
        std::shared_ptr<hl::login::login_session> session;
         if (!hl::singleton<hl::login::login_server>::get().try_get(_socket_sn, session))
             return;

        namespace sql = sqlpp::mysql;
        out_buffer out_buf(pb::ServerMessage::LoginRes);

        try
        {
            db::Users user{};
            auto tx = hl::mysql::start_transaction(conn, sqlpp::isolation_level::repeatable_read);

            auto result = conn(
                    select(user.name, user.password, user.salt, user.uid, user.sessionIp).
                            from(user).
                            where(user.name.like(_username)));
            if (!result.empty())
            {
                const auto& row = *result.begin();
                const auto hash = hl::hash::sha512(std::string(_password) + row.salt.text);
                if (hash == row.password.text)
                {
                    if (row.sessionIp.is_null())
                    {
                        try
                        {
                            conn(sql::update(user).set(user.lastConnected = std::chrono::system_clock::now()).where(user.uid == row.uid));

                            pb::AccountData account_data;
                            out_buf.write<uint8_t>(pb::LoginResult::Success);
                            account_data.set_uid(row.uid);
                            account_data.set_name(row.name);
                            out_buf.write_pb(account_data);
                        }
                        catch (const std::exception& ex)
                        {
                            LOGE << "sql error " << ex.what();
                            out_buf.write<uint8_t>(pb::LoginResult::DatabaseError);
                        }
                    }
                    else
                    {
                        out_buf.write<uint8_t>(pb::LoginResult::AlreadyConnected); // 이미 접속중
                    }
                }
                else
                {
                    out_buf.write<uint8_t>(pb::LoginResult::InvalidPassword); // 잘못된 패스워드
                }
            }
            else
            {
                out_buf.write<uint8_t>(pb::LoginResult::InvalidAccount); // 잘못된 계정
            }
            tx.commit();
        }
        catch (const sqlpp::exception& e)
        {
            LOGE << "sql error " << e.what();
            out_buf.write<uint8_t>(pb::LoginResult::DatabaseError);
        }
        session->write(out_buf);
    }
};

void hl::login::handlers::login_req::handle_packet(login_session &session, in_buffer &in_buf)
{
    const auto username = in_buf.read_str();
    const auto password = in_buf.read_str();

    session.get_server().accessor().post<login_job>(session.get_socket_sn(), username, password);
}
