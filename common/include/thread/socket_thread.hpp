//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP
#define STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl
{
    class abstract_session;

    class socket_thread
    {
    public:
    private:
        const uint32_t _id;
        std::condition_variable _cv;
        std::mutex _mutex;
        std::atomic_bool _alive;
        std::deque<std::shared_ptr<abstract_session>> _queued_sessions;
        std::thread _thread;

    public:
        explicit socket_thread(uint32_t id);
        ~socket_thread();

        void enqueue(const std::shared_ptr<abstract_session>& session);
        void stop();

        void run();
    };
}

#endif //STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP
