//
// Created by TSU on 2023-05-13.
//
#include "std.hpp"
#include "net/master_session.hpp"
#include "net/handler.hpp"
#include "handlers/setup_req.hpp"
#include "handlers/sign_out_req.hpp"
#include "handlers/migrate_region_req.hpp"
#include "handlers/on_migrate_req.hpp"
#include "handlers/region_management_res.hpp"

hl::master::login_handler::login_handler()
        : _handlers()
{
    _handlers[hl::InternalClientMessage_SetUpReq] = std::make_unique<hl::master::handlers::setup_req>();
    _handlers[hl::InternalClientMessage_MigrateRegionReq] = std::make_unique<hl::master::handlers::migrate_region_req>();
    _handlers[hl::InternalClientMessage_OnMigrateReq] = std::make_unique<hl::master::handlers::on_migrate_req>();
    _handlers[hl::InternalClientMessage_SignOutReq] = std::make_unique<hl::master::handlers::sign_out_req>();
    _handlers[hl::InternalClientMessage_RegionManagementRes] = std::make_unique<hl::master::handlers::region_management_res>();
}

void hl::master::login_handler::process(hl::master::master_session &session, in_buffer &in_buf)
{
    auto packet = static_cast<hl::InternalClientMessage>(in_buf.read<uint16_t>());

    if (packet >= InternalClientMessage_Max)
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }
    if (packet != InternalClientMessage_SetUpReq && !session.is_setup())
    {
        LOGE << "Not initialized session would be ignored any message.";
        session.close();
        return;
    }

    if (_handlers[packet])
        _handlers[packet]->handle_packet(session, in_buf);
}
