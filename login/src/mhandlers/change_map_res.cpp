//
// Created by T2SU on 2023/05/14.
//


#include "std.hpp"
#include "mhandlers/change_map_res.hpp"

void hl::login::mhandlers::change_map_res::handle_packet(master &session, in_buffer &in_buf)
{
    const auto socket_sn = in_buf.read<uint32_t>();
    const auto pid = in_buf.read<uint64_t>();
    const auto success = in_buf.read<pb::ChangeMapResult>();

    auto ps = hl::singleton<hl::login::login_server>::get().find(socket_sn);
    if (!ps)
    {
        LOGV << "socket " << socket_sn << " is not present.";
        return;
    }
    if (success != pb::ChangeMapResult_Success)
    {
        LOGV << "socket " << socket_sn << " has failed to enter game world";
        out_buffer out(pb::ServerMessage_ChangeMapRes);
        out.write<uint8_t>(success);
        ps->write(out);

        out_buffer req(hl::InternalClientMessage_SignOutReq);
        req.write(pid);
        MASTER->write(req);
        return;
    }

    const auto endpoint_address = in_buf.read_str();
    const auto endpoint_port = in_buf.read<uint16_t>();

    out_buffer out(pb::ServerMessage_ChangeMapRes);
    out.write<uint8_t>(pb::ChangeMapResult_Success);
    out.write_str(endpoint_address);
    out.write(endpoint_port);
    out.write(pid);
    ps->write(out);
}
