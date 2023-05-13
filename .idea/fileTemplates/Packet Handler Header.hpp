#parse("C File Header.h")
#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#parse("pragma once.h")

#[[#include]]# "net/${ServerType}_session.hpp"
#[[#include]]# "net/${ServerType}_server.hpp"
#[[#include]]# <net/packet_handler.hpp>

namespace hl::${ServerType}::handlers
{
    struct ${NAME} : public hl::packet_handler<${ServerType}_session>
    {
    public:
        void handle_packet(${ServerType}_session& session, in_buffer& in_buf) override;
    };
}

#[[#endif]]# //${INCLUDE_GUARD}
