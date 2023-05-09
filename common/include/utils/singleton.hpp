//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_SINGLETON_HPP
#define STREET_HOMELESS_SERVER_SINGLETON_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl
{
    template<typename T>
    class singleton
    {
    private:
        singleton() = default;
    public:
        singleton(const singleton&) = delete;
        singleton(singleton&&) = delete;
        singleton& operator=(const singleton&) = delete;
        singleton& operator=(singleton&&) = delete;

        static T& get()
        {
            static T obj;
            return obj;
        }
    };
}

#endif //STREET_HOMELESS_SERVER_SINGLETON_HPP
