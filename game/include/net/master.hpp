//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_MASTER_HPP
#define STREET_HOMELESS_SERVER_MASTER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::game
{
    class master : public abstract_session
    {
    public:
        master(server* server, uint32_t id, uint32_t socket_sn);

        void on_packet(in_buffer& in_buffer) override;
        void on_close(close_reason reason) override;
        const char* get_type_name() const override;
        void on_connect() override;
    };
}

#endif //STREET_HOMELESS_SERVER_MASTER_HPP
