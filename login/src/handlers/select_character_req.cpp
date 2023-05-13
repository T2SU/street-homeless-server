//
// Created by TSU on 2023-05-11.
//


#include "std.hpp"
#include "handlers/select_character_req.hpp"

class select_character_job : public hl::database::job
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

        out_buffer out_buf(pb::ServerMessage_SelectCharacterRes);

        try
        {
            if (session->get_account_data().uid() == 0)
                throw std::runtime_error("no session uid");

            db::Characters chr{};

            auto tx = hl::mysql::start_transaction(conn, sqlpp::isolation_level::repeatable_read);
            const auto u = conn(update(chr)
                    .set(chr.lastConnected = std::chrono::system_clock::now())
                    .where(chr.pid == _pid and chr.uid == session->get_account_data().uid()));
            if (u == 0)
            {
                out_buf.write<uint8_t>(pb::SelectCharacterResult_NoCharacter);
            }
            else
            {
                out_buf.write<uint8_t>(pb::SelectCharacterResult_Success);
            }
        }
        catch (const sqlpp::exception& sqlex)
        {
            LOGE << "sql error " << sqlex.what();
            out_buf.write<uint8_t>(pb::SelectCharacterResult_DatabaseError);
        }
        catch (const std::exception& ex)
        {
            out_buf.write<uint8_t>(pb::SelectCharacterResult_UnknownError);
            session->write(out_buf);
            throw ex;
        }

        session->write(out_buf);
    }
};

void hl::login::handlers::select_character_req::handle_packet(login_session &session, in_buffer &in_buf)
{
    if (session.get_account_data().uid() == 0)
        throw std::runtime_error("no session uid");
    session.get_server().accessor().post<select_character_job>(session.get_socket_sn(), in_buf.read<uint64_t>());
}
