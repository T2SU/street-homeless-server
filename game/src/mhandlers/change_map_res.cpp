//
// Created by T2SU on 2023/05/14.
//


#include "std.hpp"
#include "mhandlers/change_map_res.hpp"

void hl::game::mhandlers::change_map_res::handle_packet(master &session, in_buffer &in_buf)
{
    const auto socket_sn = in_buf.read<uint32_t>();
    const auto success = in_buf.read<bool>();
    const auto pid = in_buf.read<uint64_t>();
    const auto endpoint_address = in_buf.read_str();
    const auto endpoint_port = in_buf.read<uint16_t>();

    auto ps = hl::singleton<hl::game::game_server>::get().find(socket_sn);
    if (!ps)
    {
        LOGV << "socket " << socket_sn << " is not present.";
        return;
    }
    if (!success)
    {
        LOGV << "socket " << socket_sn << " has failed to change map";
        out_buffer out(pb::ServerMessage_ChangeMapRes);
        out.write<uint8_t>(0);
        ps->write(out);
        return;
    }

    out_buffer out(pb::ServerMessage_ChangeMapRes);
    out.write<uint8_t>(1);
    out.write_str(endpoint_address);
    out.write(endpoint_port);
    out.write(pid);
    ps->write(out);
}
