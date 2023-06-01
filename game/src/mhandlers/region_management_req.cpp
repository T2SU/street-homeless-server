//
// Created by TSU on 2023-05-14.
//


#include "std.hpp"
#include "mhandlers/region_management_req.hpp"
#include "maps/map_factory.hpp"
#include "maps/region_man.hpp"

void hl::game::mhandlers::region_management_req::handle_packet(master &session, in_buffer &in_buf)
{
    const auto command = in_buf.read<region_management_type>();
    const auto region_sn = in_buf.read<region_sn_t>();

    if (command == region_management_type::create)
    {
        const auto region_id = in_buf.read<region_id_t>();

        REGIONS.create(region_sn, region_id);
        out_buffer obuf(hl::InternalClientMessage_RegionManagementRes);
        obuf.write(region_sn);
        obuf.write(true);
        session.write(obuf);
    }
    else if (command == region_management_type::remove)
    {
        REGIONS.remove(region_sn);
    }
}
