#parse("C File Header.h")
#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#parse("pragma once.h")

#[[#include]]# "net/login_session.hpp"
#[[#include]]# "net/login_server.hpp"
#[[#include]]# <net/packet_handler.hpp>

namespace hl::login::handlers
{
    struct ${NAME} : public hl::packet_handler<login_session>
    {
    public:
        void handle_packet(login_session& session, in_buffer& in_buf) override;
    };
}

#[[#endif]]# //${INCLUDE_GUARD}
