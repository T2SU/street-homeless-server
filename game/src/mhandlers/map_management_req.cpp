//
// Created by TSU on 2023-05-14.
//


#include "std.hpp"
#include "mhandlers/map_management_req.hpp"
#include "maps/map_factory.hpp"

void hl::game::mhandlers::map_management_req::handle_packet(master &session, in_buffer &in_buf)
{
    const auto command = in_buf.read<map_management_type>();
    const auto map_sn = in_buf.read<uint32_t>();
    const auto scene = in_buf.read_str();
    const auto type = in_buf.read<map_type>();

    if (command == map_management_type::create)
    {
        auto result = MAPS.create_map(map_sn, scene, type);
        out_buffer obuf(hl::InternalClientMessage_MapManagementRes);
        obuf.write(map_sn);
        obuf.write(result);
        session.write(obuf);
    }
    else if (command == map_management_type::remove)
    {
        MAPS.remove_map(map_sn);
    }
}
