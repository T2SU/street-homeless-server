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
        hl::singleton<map_factory>::get().create_map(map_sn, scene, type);
    }
    else if (command == map_management_type::remove)
    {
        hl::singleton<map_factory>::get().remove_map(map_sn);
    }
}
