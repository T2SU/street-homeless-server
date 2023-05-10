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

    namespace sql = sqlpp::postgresql;

    auto config = std::make_shared<sql::connection_config>();
    config->user = "postgres";
    config->password = "fndndnxm";
    config->dbname = "homeless";
    config->debug = true;
    try
    {
        sql::connection db(config);
        db::PublicUsers user;

        auto result = db(select(user.name, user.password, user.salt, user.uid, user.sessionIp).from(user).where(user.name.like(username)));
        if (result.size() == 0)
        {
            out_buf.write<uint8_t>(pb::LoginResult::InvalidAccount);
        }
        else
        {
            const auto& row = *result.begin();

        }

        if (username != "test")
            out_buf.write<uint8_t>(pb::LoginResult::InvalidAccount);
        else if (password != "test")
            out_buf.write<uint8_t>(pb::LoginResult::InvalidPassword);
        else
        {
            pb::AccountData account_data;
            out_buf.write<uint8_t>(pb::LoginResult::Success);
            account_data.set_uid(100);
            account_data.set_name(username);
            out_buf.write_pb(account_data);
        }
    }
    catch (const sqlpp::exception& e)
    {
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
