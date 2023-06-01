//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_ABSTRACT_REQUEST_HPP
#define STREET_HOMELESS_SERVER_ABSTRACT_REQUEST_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::master
{
    class user_record;
    class master_session;

    class abstract_request
    {
    private:
        const socket_sn_t _master_socket_sn;
        const player_id_t _pid;

    public:
        abstract_request(socket_sn_t master_socket_sn, player_id_t pid);

        [[nodiscard]] std::shared_ptr<master_session> get_session() const;
        [[nodiscard]] std::shared_ptr<user_record> get_user() const;

        [[nodiscard]] inline player_id_t get_pid() const { return _pid; }

        [[nodiscard]] out_buffer make_reply(hl::InternalServerMessage msg) const;
    };
}

#endif //STREET_HOMELESS_SERVER_ABSTRACT_REQUEST_HPP
