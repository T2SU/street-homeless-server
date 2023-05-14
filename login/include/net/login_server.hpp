//
// Created by TSU on 2023-05-04.
//

#ifndef STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP
#define STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "login_session.hpp"

namespace hl::login
{
    class login_server : public abstract_server<login_session>
    {
    private:
        std::vector<std::shared_ptr<hl::database::accessor>> _accessor;
        size_t _accessor_count;
        std::atomic_size_t _accessor_robin;
        uint32_t _idx;

    public:
        login_server();

        hl::database::accessor& accessor();
        void on_accept(login_session* session) override;
        void encode_config(out_buffer &obuf);
    };
}

#endif //STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP
