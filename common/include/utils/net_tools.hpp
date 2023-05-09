//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_NET_TOOLS_HPP
#define STREET_HOMELESS_SERVER_NET_TOOLS_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl
{
    class net_tools {
    public:
        net_tools() = delete;
        virtual ~net_tools() = delete;
        net_tools(const net_tools&) = delete;
        net_tools(net_tools&&) = delete;
        net_tools& operator= (const net_tools&) = delete;
        net_tools& operator= (net_tools&&) = delete;

        static std::string dump_memory(const void* buffer, size_t len);
    };
}

#endif //STREET_HOMELESS_SERVER_NET_TOOLS_HPP
