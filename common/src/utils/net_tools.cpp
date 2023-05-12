//
// Created by TSU on 2023-05-03.
//

#include "std_common.hpp"
#include "utils/net_tools.hpp"

std::string hl::net_tools::dump_memory(const void *buffer, size_t len)
{
    const char*         buf = reinterpret_cast<const char*>(buffer);
    std::stringstream   ss;

    while (len-- != 0)
    {
        const auto x = *(buf++);
        ss << std::format("{:02X}", static_cast<unsigned char>(x));
        if (len == 0)
            break;
        ss << ' ';
    }
    return ss.str();
}

size_t hl::net_tools::get_utf8_length(const std::string &s)
{
    std::size_t len = 0;
    auto begin = s.cbegin();
    auto end = s.cend();
    while (begin != end) {
        unsigned char c = *begin;
        if ((c & 0xC0) != 0x80) {
            len++;
        }
        begin++;
    }
    return len;
}

bool hl::net_tools::check_regex(const std::string &str, const std::string &re)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv; // TODO more reliable source
    std::wstring wstr(myconv.from_bytes(str));
    std::wstring wre(myconv.from_bytes(re));

    std::wregex we(wre);
    return std::regex_search(wstr, we);
}
