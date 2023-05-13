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

    public:
        login_server()
        : abstract_server()
        , _accessor()
        , _accessor_count()
        , _accessor_robin()
        {
            const auto config = hl::yaml::load("login.yaml");
            const auto accessor_count = config["thread"]["db_accessor_count"].as<size_t>(1);
            for (size_t i = 0; i < accessor_count; i++)
            {
                _accessor.push_back(std::make_shared<hl::database::accessor>());
            }
            _accessor_count = accessor_count;
        }

        hl::database::accessor& accessor();
        void on_accept(login_session* session) override;
    };
}

#endif //STREET_HOMELESS_SERVER_LOGIN_SERVER_HPP
