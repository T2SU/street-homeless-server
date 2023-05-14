//
// Created by TSU on 2023-05-13.
//


#include "std.hpp"
#include "handlers/setup_req.hpp"
#include "net/master_server.hpp"
#include "world/game_world.hpp"

void hl::master::handlers::setup_req::handle_packet(master_session &session, in_buffer &in_buf)
{
    if (session.is_setup())
        throw std::runtime_error("Already set up session.");
    const auto type = in_buf.read<server_type>();
    switch (type)
    {
        case server_type::game:
        {
            const auto idx = in_buf.read<uint8_t>();
            const auto flag = in_buf.read<game_server_flag>();
            const auto endpoint_address = in_buf.read_str();
            const auto endpoint_port = in_buf.read<uint16_t>();

            session.set_server_type(server_type::game, idx, flag);
            session.set_endpoint(endpoint_address, endpoint_port);
            hl::singleton<hl::master::game_world>::get().add_server(idx, flag);
            break;
        }
        case server_type::login:
            session.set_server_type(server_type::login, 0, 0);
            break;
    }
    hl::singleton<hl::master::master_server>::get().add(session);
}
