//
// Created by TSU on 2023-05-13.
//


#include "std.hpp"
#include "handlers/setup_req.hpp"

void hl::master::handlers::setup_req::handle_packet(master_session &session, in_buffer &in_buf)
{
    if (session.is_setup())
        throw std::runtime_error("Already set up session.");
    const auto type = in_buf.read<server_type>();
    switch (type)
    {
        case server_type::game:
            session.set_server_type(server_type::game, in_buf.read<uint8_t>(), in_buf.read<game_server_flag>());
            break;
        case server_type::login:
            session.set_server_type(server_type::login, 0, 0);
            break;
    }
    
}
