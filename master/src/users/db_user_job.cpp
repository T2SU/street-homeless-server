//
// Created by TSU on 2023-05-14.
//

#include "std.hpp"
#include "users/user_record.hpp"
#include "users/db_user_job.hpp"

hl::master::db_user_job::db_user_job(std::shared_ptr<hl::master::user_record> user)
        : _user(std::move(user))
{
}

void hl::master::db_user_job::process(sqlpp::mysql::connection &conn)
{
    _user->process_db_job(conn);
}