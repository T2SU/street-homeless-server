//
// Created by TSU on 2023-05-12.
//

#include "std.hpp"
#include "handlers/create_character_req.hpp"

class create_character_job : public hl::database::job
{
private:
    const uint32_t _socket_sn;
    std::string _name;
    pb::PlayerAppearance _pa;

public:
    create_character_job(const uint32_t socket_sn, in_buffer& in_buf)
        : _socket_sn(socket_sn)
    {
        _name = in_buf.read_str();
        in_buf.read_pb(_pa);
    }

    void process(sqlpp::mysql::connection& conn) override
    {
        std::shared_ptr<hl::login::login_session> session;
        if (!hl::singleton<hl::login::login_server>::get().try_get(_socket_sn, session))
            return;

        namespace sql = sqlpp::mysql;
        out_buffer out_buf(pb::ServerMessage_CreateCharacterRes);

        try
        {
            if (session->get_account_data().uid() == 0)
                throw std::runtime_error("no session uid");

            db::Characters chr{};
            const auto name_length = hl::net_tools::get_utf8_length(_name);
            if (name_length < 2 || name_length > 8)
                throw std::runtime_error("wrong name length (" + std::to_string(name_length) + ")");
            if (!hl::net_tools::check_regex(_name, "^[a-zA-Z0-9가-힣]+$"))
                throw std::runtime_error("wrong name pattern (" + _name + ")");

            if (_pa.hair() < 0 || _pa.hair() > 4)
                throw std::runtime_error("wrong hair index (" + std::to_string(_pa.hair()) + ")");
            if (_pa.hat_index() < -1 || _pa.hat_index() > 2)
                throw std::runtime_error("wrong hat index (" + std::to_string(_pa.hat_index()) + ")");
            if (_pa.shirt_index() < 0 || _pa.shirt_index() > 9)
                throw std::runtime_error("wrong shirt index (" + std::to_string(_pa.shirt_index()) + ")");
            if (_pa.pants_index() < 0 || _pa.pants_index() > 9)
                throw std::runtime_error("wrong pants index (" + std::to_string(_pa.pants_index()) + ")");
            if (_pa.shoes_index() < 0 || _pa.shoes_index() > 9)
                throw std::runtime_error("wrong shoes index (" + std::to_string(_pa.shoes_index()) + ")");
            if (_pa.beard() < -1 || _pa.beard() > 9)
                throw std::runtime_error("wrong beard index (" + std::to_string(_pa.beard()) + ")");
            if (_pa.gender() != 0 && _pa.gender() != 1)
                throw std::runtime_error("wrong gender (" + std::to_string(_pa.gender()) + ")");
            if (_pa.fat() < 0 || _pa.fat() > 100)
                throw std::runtime_error("wrong fat weight (" + std::to_string(_pa.fat()) + ")");
            if (_pa.muscles() < 0 || _pa.muscles() > 100)
                throw std::runtime_error("wrong muscles weight (" + std::to_string(_pa.muscles()) + ")");
            if (_pa.slimness() < 0 || _pa.slimness() > 100)
                throw std::runtime_error("wrong slimness weight (" + std::to_string(_pa.slimness()) + ")");
            if (_pa.breast() < 0 || _pa.breast() > 100)
                throw std::runtime_error("wrong breast weight (" + std::to_string(_pa.breast()) + ")");

            auto tx = hl::mysql::start_transaction(conn, sqlpp::isolation_level::repeatable_read);

            const auto character_num_row = conn(
                    select(count(chr.pid))
                    .from(chr)
                    .where(chr.uid == session->get_account_data().uid())
                    );
            if (character_num_row.front().count > 3)
            {
                out_buf.write<uint8_t>(pb::CreateCharacterResult_UnknownError);
                return;
            }

            const auto name_row = conn(
                    select(count(chr.pid))
                    .from(chr)
                    .where(chr.name.like(_name)));
            if (name_row.front().count > 0)
            {
                out_buf.write<uint8_t>(pb::CreateCharacterResult_DuplicatedName);
                session->write(out_buf);
                return;
            }

            conn(insert_into(chr).set(
                    chr.uid = session->get_account_data().uid(),
                    chr.name = _name,
                    chr.health = 100,
                    chr.tiredness = 0,
                    chr.maxHealth = 100,
                    chr.maxTiredness = 100,
                    chr.hair = _pa.hair(),
                    chr.hairColor = (_pa.hair_color() & 0xffffff),
                    chr.shirt = _pa.shirt_index(),
                    chr.pants = _pa.pants_index(),
                    chr.shoes = _pa.shoes_index(),
                    chr.hat = _pa.hat_index(),
                    chr.beard = _pa.beard(),
                    chr.gender = _pa.gender(),
                    chr.fat = _pa.fat(),
                    chr.muscle = _pa.muscles(),
                    chr.slimness = _pa.slimness(),
                    chr.breast = _pa.breast(),
                    chr.money = 0));
            tx.commit();
            out_buf.write<uint8_t>(pb::CreateCharacterResult_Success);
            session->write(out_buf);
        }
        catch (const sqlpp::exception& e)
        {
            LOGE << "sql error " << e.what();
            out_buf.write<uint8_t>(pb::CreateCharacterResult_DatabaseError);
            session->write(out_buf);
        }
        catch (const std::exception& e)
        {
            LOGD << "create character generalized error " << e.what();
            out_buf.write<uint8_t>(pb::CreateCharacterResult_UnknownError);
            session->write(out_buf);
            throw e;
        }
    }
};

void hl::login::handlers::create_character_req::handle_packet(login_session &session, in_buffer &in_buf)
{
    if (session.get_account_data().uid() == 0)
        throw std::runtime_error("no session uid");
    session.get_server().accessor().post<create_character_job>(session.get_socket_sn(), in_buf);
}
