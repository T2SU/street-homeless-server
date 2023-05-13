//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_MASTER_SESSION_HPP
#define STREET_HOMELESS_SERVER_MASTER_SESSION_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::master
{
    class master_server;
    class master_session : public abstract_session
    {
    private:
        server_type _type;
        uint32_t _idx;
        uint32_t _game_flag;
        bool _setup;

    public:
        master_session(server* server, uint32_t id, uint32_t socket_sn);

        void on_packet(in_buffer& in_buffer) override;
        void on_close(close_reason reason) override;
        const char* get_type_name() const override;

        bool is_setup() const;
        void set_server_type(server_type type, uint32_t idx, uint32_t game_flag);

        master_server& get_server();
    };
}

#endif //STREET_HOMELESS_SERVER_MASTER_SESSION_HPP
