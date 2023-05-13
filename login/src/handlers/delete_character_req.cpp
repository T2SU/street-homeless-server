//
// Created by TSU on 2023-05-13.
//

#include "std.hpp"
#include "handlers/delete_character_req.hpp"

class delete_character_job : public hl::database::job
{
private:
    const uint32_t _socket_sn;
    const uint64_t _pid;

public:
    explicit delete_character_job(const hl::login::login_session& session, uint64_t pid)
            : _socket_sn(session.get_socket_sn())
            , _pid(pid){}

    void process(sqlpp::mysql::connection& conn) override
    {
        std::shared_ptr<hl::login::login_session> session;
        if (!hl::singleton<hl::login::login_server>::get().try_get(_socket_sn, session))
            return;

        out_buffer out_buf(pb::ServerMessage_DeleteCharacterRes);

        try
        {
            if (session->get_account_data().uid() == 0)
                throw std::runtime_error("no session uid");

            db::Characters chr{};

            auto ret = conn(remove_from(chr)
                    .where(chr.pid == _pid and chr.uid == session->get_account_data().uid()));
            if (ret == 1)
                out_buf.write<uint8_t>(pb::DeleteCharacterResult_Success);
            else
                out_buf.write<uint8_t>(pb::DeleteCharacterResult_NoCharacter);
        }
        catch (const sqlpp::exception& sqlex)
        {
            LOGE << "sql error " << sqlex.what();
            out_buf.write<uint8_t>(pb::CharacterListResult_DatabaseError);
        }
        catch (const std::exception& ex)
        {
            out_buf.write<uint8_t>(pb::CharacterListResult_UnknownError);
            session->write(out_buf);
            throw ex;
        }
        session->write(out_buf);
    }
};

void hl::login::handlers::delete_character_req::handle_packet(login_session &session, in_buffer &in_buf)
{
    if (session.get_account_data().uid() == 0)
        throw std::runtime_error("no session uid");
    session.get_server().accessor().post<delete_character_job>(session, in_buf.read<uint64_t>());
}
