//
// Created by TSU on 2023-05-13.
//


#include "std.hpp"
#include "handlers/character_list_req.hpp"

class character_list_job : public hl::database::job
{
private:
    const uint32_t _socket_sn;

public:
    explicit character_list_job(const hl::login::login_session& session)
        : _socket_sn(session.get_socket_sn()) {}

    void process(sqlpp::mysql::connection& conn) override
    {
        std::shared_ptr<hl::login::login_session> session;
        if (!hl::singleton<hl::login::login_server>::get().try_get(_socket_sn, session))
            return;

        out_buffer out_buf(pb::ServerMessage_CharacterListRes);

        try
        {
            if (session->get_account_data().uid() == 0)
                throw std::runtime_error("no session uid");

            db::Characters chr{};

            auto rows = conn(select(chr.pid,
                                    chr.name,
                                    chr.hair,
                                    chr.hairColor,
                                    chr.shirt,
                                    chr.pants,
                                    chr.shoes,
                                    chr.hat,
                                    chr.beard,
                                    chr.gender,
                                    chr.fat,
                                    chr.muscle,
                                    chr.slimness,
                                    chr.breast,
                                    chr.money)
                                          .from(chr)
                                          .where(chr.uid == session->get_account_data().uid())
                                          .limit(3u));

            out_buf.write<uint8_t>(pb::CharacterListResult_Success);
            for (const auto& row : rows)
            {
                out_buf.write<bool>(true);
                out_buf.write<uint64_t>(row.pid);
                out_buf.write_str(row.name);

                pb::PlayerAppearance pa;
                pa.set_hair(row.hair);
                pa.set_hair_color(row.hairColor);
                pa.set_shirt_index(row.shirt);
                pa.set_pants_index(row.pants);
                pa.set_shoes_index(row.shoes);
                pa.set_hat_index(row.hat);
                pa.set_beard(row.beard);
                pa.set_gender(row.gender);
                pa.set_fat(row.fat);
                pa.set_muscles(row.muscle);
                pa.set_slimness(row.slimness);
                pa.set_breast(row.breast);
                out_buf.write_pb(pa);
            }
            out_buf.write<bool>(false);
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

void hl::login::handlers::character_list_req::handle_packet(login_session &session, in_buffer &in_buf)
{
    session.get_server().accessor().post<character_list_job>(session);
}
