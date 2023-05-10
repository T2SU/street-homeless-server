//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_HASH_HPP
#define STREET_HOMELESS_SERVER_HASH_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::hash
{
    std::string sha1(const void* ptr, size_t len);
    std::string sha1(const std::string& str);

    std::string sha512(const void* ptr, size_t len);
    std::string sha512(const std::string& str);
}

#endif //STREET_HOMELESS_SERVER_HASH_HPP
