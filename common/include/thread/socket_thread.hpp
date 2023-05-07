//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP
#define STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP

namespace homeless
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
        std::deque<abstract_session*> _queued_sessions;

    public:
        explicit socket_thread(uint32_t id);
        ~socket_thread();

        void enqueue(abstract_session* session);
        void stop();

        static void run(socket_thread* inst);
    };
}

#endif //STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP
