//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_MYSQL_TRANSACTION_HPP
#define STREET_HOMELESS_SERVER_MYSQL_TRANSACTION_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::mysql
    {
        template<typename Db>
        sqlpp::transaction_t<Db> start_transaction(Db& db,
                                    sqlpp::isolation_level isolation,
                                    bool report_unfinished_transaction = sqlpp::report_auto_rollback)
        {
            switch (isolation)
            {
                case sqlpp::isolation_level::serializable:
                    db.execute("SET TRANSACTION ISOLATION LEVEL SERIALIZABLE");
                    break;
                case sqlpp::isolation_level::repeatable_read:
                    db.execute("SET TRANSACTION ISOLATION LEVEL REPEATABLE READ");
                    break;
                case sqlpp::isolation_level::read_committed:
                    db.execute("SET TRANSACTION ISOLATION LEVEL READ COMMITTED");
                    break;
                case sqlpp::isolation_level::read_uncommitted:
                    db.execute("SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED");
                    break;
                default:
                    break;
            }
            return {db, report_unfinished_transaction};
        }
}


#endif //STREET_HOMELESS_SERVER_MYSQL_TRANSACTION_HPP
