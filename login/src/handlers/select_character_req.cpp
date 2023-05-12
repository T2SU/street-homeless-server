//
// Created by TSU on 2023-05-11.
//


#include "std.hpp"
#include "handlers/select_character_req.hpp"

class select_character_job : hl::database::job
{
private:
    const uint32_t _socket_sn;
    const uint64_t _pid;

public:
    select_character_job(uint32_t socket_sn, uint64_t pid)
        : _socket_sn(socket_sn), _pid(pid) {}

    void process(sqlpp::mysql::connection& conn) override
    {
        std::shared_ptr<hl::login::login_session> session;
        if (!hl::singleton<hl::login::login_server>::get().try_get(_socket_sn, session))
            return;



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

        out_buffer out_buf(pb::ServerMessage_EnterGameWorldRes);
        out_buf.write_pb(playerData);
        session->write(out_buf);

        playerData.release_inventory();
        playerData.release_stat();
    }
};

void hl::login::handlers::select_character_req::handle_packet(login_session &session, in_buffer &in_buf)
{

}
