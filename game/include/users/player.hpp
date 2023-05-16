//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_E004541560BA448BB32EC3266A7EA785
#define STREET_HOMELESS_SERVER_E004541560BA448BB32EC3266A7EA785

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/game_session.hpp"
#include "player_stat.hpp"
#include "items/item.hpp"
#include "objects/field_object.hpp"

namespace hl::game
{
    class map;

    class player : field_object
    {
    private:
        uint64_t _pid;
        uint64_t _uid;
        player_stat _stat;
        int32_t _money;
        std::vector<item> _inventory;
        std::string _name;
        uint32_t _map_sn;

        vector3 _position;
        quaternion _rotation;

        std::weak_ptr<game_session> _session;

    public:
        player(uint64_t pid, const std::shared_ptr<game_session> session);

        void init(pb::PlayerData& data, uint32_t map_sn);
        void on_close();

        uint64_t get_pid() const;
        uint64_t get_uid() const;
        const std::string &get_name() const;
        std::shared_ptr<game_session> get_session() const;
        map* get_map() const;
        uint32_t get_map_id() const;

        const vector3 &get_position() const;
        void set_position(vector3 position);
        const quaternion &get_rotation() const;
        void set_rotation(quaternion rotation);

        void encode_enter(out_buffer& out) const override;
        void encode_leave(out_buffer& out) const override;

        void send_enter(const std::shared_ptr<field_object>& obj);
        void send_leave(const std::shared_ptr<field_object>& obj);

        std::string to_string() const override;

    private:
        void init_inventory(pb::PlayerData& data);
    };
}

#endif //STREET_HOMELESS_SERVER_E004541560BA448BB32EC3266A7EA785
