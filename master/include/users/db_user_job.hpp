//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_A6E7EE9A83B443C68ED4243E086E4849
#define STREET_HOMELESS_SERVER_A6E7EE9A83B443C68ED4243E086E4849

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::master
{
    class user_record;

    class db_user_job : public hl::database::job
    {
    private:
        std::shared_ptr<user_record> _user;

    public:
        explicit db_user_job(std::shared_ptr<user_record> user);

        void process(sqlpp::mysql::connection &conn) override;
    };
}

#endif //STREET_HOMELESS_SERVER_A6E7EE9A83B443C68ED4243E086E4849
