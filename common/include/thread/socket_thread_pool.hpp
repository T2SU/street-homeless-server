//
// Created by TSU on 2023-05-08.
//

#ifndef STREET_HOMELESS_SERVER_SOCKET_THREAD_POOL_HPP
#define STREET_HOMELESS_SERVER_SOCKET_THREAD_POOL_HPP

#include "socket_thread.hpp"

namespace homeless
{
    class socket_thread_pool
    {
    private:
        std::vector<std::shared_ptr<homeless::socket_thread>> _threads;
        mutable std::atomic_size_t _idx;

    public:
        explicit socket_thread_pool();

        void begin(uint32_t threads = 4);
        void end();

        std::shared_ptr<homeless::socket_thread> get_worker_thread() const;
    };
}

#endif //STREET_HOMELESS_SERVER_SOCKET_THREAD_POOL_HPP
