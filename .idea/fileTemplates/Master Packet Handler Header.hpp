#parse("C File Header.h")
#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#parse("pragma once.h")

#[[#include]]# "net/master.hpp"
#[[#include]]# "net/${ServerType}_server.hpp"
#[[#include]]# <net/packet_handler.hpp>

namespace hl::${ServerType}::mhandlers
{
    struct ${NAME} : public hl::packet_handler<master>
    {
    public:
        void handle_packet(master& session, in_buffer& in_buf) override;
    };
}

#[[#endif]]# //${INCLUDE_GUARD}
