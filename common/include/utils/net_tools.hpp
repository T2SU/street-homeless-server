//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_NET_TOOLS_HPP
#define STREET_HOMELESS_SERVER_NET_TOOLS_HPP

namespace homeless
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
