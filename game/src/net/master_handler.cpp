//
// Created by TSU on 2023-05-11.
//
#include "std.hpp"
#include "net/master.hpp"
#include "net/master_handler.hpp"
#include "mhandlers/region_management_req.hpp"
#include "mhandlers/on_migrate_res.hpp"
#include "mhandlers/migrate_region_res.hpp"

hl::game::master_handler::master_handler()
    : _handlers()
{
    _handlers[hl::InternalServerMessage_RegionManagementReq] = std::make_unique<hl::game::mhandlers::region_management_req>();
    _handlers[hl::InternalServerMessage_OnMigrateRes] = std::make_unique<hl::game::mhandlers::on_migrate_res>();
    _handlers[hl::InternalServerMessage_MigrateRegionRes] = std::make_unique<hl::game::mhandlers::migrate_region_res>();
}

void hl::game::master_handler::process(hl::game::master &session, in_buffer &in_buf)
{
    auto packet = static_cast<hl::InternalServerMessage>(in_buf.read<uint16_t>());

    if (packet >= InternalServerMessage_Max)
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }

    if (_handlers[packet])
        _handlers[packet]->handle_packet(session, in_buf);
}
