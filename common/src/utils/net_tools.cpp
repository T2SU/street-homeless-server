//
// Created by TSU on 2023-05-03.
//

#include "std_common.hpp"
#include "utils/net_tools.hpp"

std::string homeless::net_tools::dump_memory(const void *buffer, size_t len)
{
    const char*         buf = reinterpret_cast<const char*>(buffer);
    std::stringstream   ss;

    while (len-- != 0)
    {
        const auto x = *(buf++);
        ss << std::format("{:02X}", static_cast<unsigned long>(x));
        if (len == 0)
            break;
        ss << ' ';
    }
    return ss.str();
}
