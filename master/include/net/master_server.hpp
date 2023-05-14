//
// Created by TSU on 2023-05-13.
//

#ifndef STREET_HOMELESS_SERVER_MASTER_SERVER_HPP
#define STREET_HOMELESS_SERVER_MASTER_SERVER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "master_session.hpp"

namespace hl::master
{
    class master_server : public abstract_server<master_session>
    {
    private:
        std::vector<std::shared_ptr<hl::database::accessor>> _accessor;
        size_t _accessor_count;
        std::atomic_size_t _accessor_robin;
        mutable std::recursive_mutex _mutex;
        std::unordered_map<server_type, std::unordered_map<uint32_t, std::shared_ptr<master_session>>> _sessions;

    public:
        master_server();

        hl::database::accessor& accessor();
        void on_accept(master_session* session) override;
        void add(master_session& session);
        void remove(master_session& session);
        using abstract_server<master_session>::try_get;
        bool try_get(server_type type, uint32_t id, std::shared_ptr<master_session>& ptr) const;
        auto get(server_type type, uint32_t id) const -> std::shared_ptr<master_session>;

        inline auto sbegin(server_type type) { return _sessions.begin(); }
        inline auto send(server_type type) { return _sessions.end(); }
    };
}

#endif //STREET_HOMELESS_SERVER_MASTER_SERVER_HPP
