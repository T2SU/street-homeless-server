//
// Created by TSU on 2023-05-07.
//

#ifndef STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP
#define STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP

enum class socket_op { READ, WRITE, CLOSE };

class abstract_session;
class in_buffer;

class socket_thread
{
public:
    typedef struct socket_job
    {
        socket_op op;
        abstract_session* socket;
        in_buffer* packet;
    } socket_job;

private:
    const uint32_t _id;
    std::condition_variable _cv;
    std::mutex _mutex;
    std::atomic_bool _alive;
    std::deque<socket_job> _jobs;

public:
    explicit socket_thread(uint32_t id);
    ~socket_thread();

    void enqueue(socket_job job);
    void stop();

    static void run(socket_thread* inst);
};

#endif //STREET_HOMELESS_SERVER_SOCKET_THREAD_HPP
