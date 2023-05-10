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
    const auto op = in_buffer.read<uint16_t>();
    switch (op)
    {
        case pb::ClientMessage::CheckAliveRes:
            on_check_alive_res(in_buffer);
            break;
        case pb::ClientMessage::LoginReq:
            on_login_req(in_buffer);
            break;
        case pb::ClientMessage::SelectCharacterReq:
            on_select_character_req(in_buffer);
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

//    namespace sql = sqlpp::postgresql;
//    auto config = std::make_shared<sql::connection_config>();
//    config->user = "postgres";
//    config->password = "fndndnxm";
//    config->dbname = "homeless";
//    config->debug = true;
    namespace sql = sqlpp::mysql;
    auto config = std::make_shared<sql::connection_config>();
    config->user = "root";
    config->password = "fndndnxm";
    config->database = "homeless";
    config->port = 3306;
    config->debug = true;

    try
    {
        db::Users user{};
        sql::connection db(config);
        auto tx = hl::mysql::start_transaction(db, sqlpp::isolation_level::repeatable_read);

        auto result = db(
                select(user.name, user.password, user.salt, user.uid, user.sessionIp).
                from(user).
                where(user.name.like(username)));
        if (!result.empty())
        {
            const auto& row = *result.begin();
            const auto hash = hl::hash::sha512(std::string(password) + row.salt.text);
            if (hash == row.password.text)
            {
                if (row.sessionIp.is_null())
                {
                    try
                    {
                        db(sql::update(user).set(user.sessionIp = get_remote_address()).where(user.uid == row.uid));

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

    write(out_buf);
}

void hl::login::login_session::on_select_character_req(in_buffer &in_buf)
{
    pb::PlayerData playerData;
    pb::PlayerStat playerStat;
    pb::Inventory inventory;

    playerStat.set_health(100);
    playerStat.set_tiredness(0);
    playerStat.set_max_health(100);
    playerStat.set_max_tiredness(100);

    inventory.set_money(860);
    const std::vector<std::pair<int ,int>> test_items(
    {
        {40000000, 10}, // 박스떼기
        {40000001, 54}, // 노숙일보
        {30000000, 3}, // 뚜껑없는 깡통
        {20000000, 1}, // LPG(소)
    });

    auto sn = 0;
    for (const auto& test_item : test_items)
    {
        auto i = inventory.add_items();
        i->set_sn(sn++);
        i->set_item_id(test_item.first);
        i->set_amount(test_item.second);
    }

    playerData.set_allocated_inventory(&inventory);
    playerData.set_allocated_stat(&playerStat);
    playerData.set_name("김철남");
    playerData.set_pid(1000);

    out_buffer out_buf(pb::ServerMessage::SetStage);
    out_buf.write_pb(playerData);
    write(out_buf);

    playerData.release_inventory();
    playerData.release_stat();
}
