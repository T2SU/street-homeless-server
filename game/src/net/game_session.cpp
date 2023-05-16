//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/game_session.hpp"
#include "net/game_server.hpp"
#include "net/game_handler.hpp"
#include "net/master.hpp"
#include "users/player.hpp"

hl::game::game_session::game_session(server* server, uint32_t id, uint32_t socket_sn)
        : abstract_session(server, id, socket_sn)
        , _player()
        , _state(user_state::migrating)
        , _migrating_to_another(false)
{}

void hl::game::game_session::on_close(close_reason reason)
{
    try_sign_out();
}

void hl::game::game_session::on_packet(in_buffer &in_buffer)
{
    hl::singleton<hl::game::game_handler>::get().process(*this, in_buffer);
}

hl::game::game_server &hl::game::game_session::get_server()
{
    return *reinterpret_cast<hl::game::game_server*>(_server);
}

const char *hl::game::game_session::get_type_name() const
{
    return "game_session";
}

const std::shared_ptr<hl::game::player> &hl::game::game_session::get_player() const
{
    return _player;
}

void hl::game::game_session::set_player(const std::shared_ptr<player> &player)
{
    _player = player;
}

user_state hl::game::game_session::get_state() const
{
    return _state;
}

void hl::game::game_session::set_state(user_state state)
{
    if (_state == user_state::connected && state == user_state::migrating)
        _migrating_to_another = true;
    _state = state;
}

void hl::game::game_session::try_sign_out()
{
    if (_migrating_to_another) return;
    if (_state != user_state::connected) return;
    if (!_player) return;

    _player->on_close();

    out_buffer obuf(hl::InternalClientMessage_SignOutReq);
    obuf.write(_player->get_pid());
    MASTER->write(obuf);
}
