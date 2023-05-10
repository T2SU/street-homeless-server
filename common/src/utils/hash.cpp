//
// Created by TSU on 2023-05-11.
//
#include "std_common.hpp"
#include "utils/hash.hpp"

template<size_t N>
static std::string convert_to_hex(unsigned char(&buf)[N])
{
    const auto hex = "0123456789abcdef";
    char tmp[N * 2 + 1];

    for (size_t i = 0; i < N; i++)
    {
        tmp[(i << 1) + 0] = hex[buf[i] >> 4];
        tmp[(i << 1) + 1] = hex[buf[i] & 0xf];
    }
    tmp[N * 2] = 0;
    return tmp;
}

std::string hl::hash::sha1(const void* ptr, size_t len)
{
    unsigned char md[20];

    SHA1(reinterpret_cast<const unsigned char*>(ptr), len, md);
    return convert_to_hex(md);
}

std::string hl::hash::sha1(const std::string& str)
{
    return sha1(str.data(), str.length());
}

std::string hl::hash::sha512(const void* ptr, size_t len)
{
    unsigned char md[64];

    SHA512(reinterpret_cast<const unsigned char*>(ptr), len, md);
    return convert_to_hex(md);
}

std::string hl::hash::sha512(const std::string& str)
{
    return sha512(str.data(), str.length());
}