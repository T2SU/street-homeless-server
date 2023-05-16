//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_GAME_SESSION_HPP
#define STREET_HOMELESS_SERVER_GAME_SESSION_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::game
{
    class game_server;
    class player;
    class game_session : public abstract_session
    {
    private:
        std::shared_ptr<player> _player;
        user_state _state;
        bool _migrating_to_another;

    public:
        game_session(server* server, uint32_t id, uint32_t socket_sn);

        void on_packet(in_buffer& in_buffer) override;
        void on_close(close_reason reason) override;
        const char* get_type_name() const override;
        const std::shared_ptr<player> &get_player() const;
        void set_player(const std::shared_ptr<player> &player);
        user_state get_state() const;
        void set_state(user_state state);

        inline bool is_migrating_to_another() const { return _migrating_to_another; }

        game_server& get_server();

    private:
        void try_sign_out();
    };
}

#endif //STREET_HOMELESS_SERVER_GAME_SESSION_HPP
