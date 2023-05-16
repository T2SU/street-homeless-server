//
// Created by T2SU on 2023/05/16.
//

#ifndef STREET_HOMELESS_SERVER_3658CF8A2199421AA916DCD19A42725A
#define STREET_HOMELESS_SERVER_3658CF8A2199421AA916DCD19A42725A

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl
{
    class random
    {
    private:
        mutable std::mt19937_64 _gen;
        mutable std::atomic_flag _lock = ATOMIC_FLAG_INIT;

    public:
        random();
        explicit random(uint32_t seed);

        template<typename T>
        T next() const
        {
            std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min());
            lock();
            const auto r = dist(_gen);
            unlock();
            return r;
        }

        template<typename T>
        T next(T i) const // i is inclusive max
        {
            std::uniform_int_distribution<T> dist(0, i);
            lock();
            const auto r = dist(_gen);
            unlock();
            return r;
        }

        template<typename T>
        T range(T min, T max) const // max is inclusive
        {
            std::uniform_int_distribution<T> dist(min, max);
            lock();
            const auto r = dist(_gen);
            unlock();
            return r;
        }

    private:
        inline bool lock() const
        {
            while (_lock.test_and_set(std::memory_order_acquire));
            return true;
        }
        inline void unlock() const { _lock.clear(std::memory_order_release); }
    };
} // hl

#endif //STREET_HOMELESS_SERVER_3658CF8A2199421AA916DCD19A42725A
