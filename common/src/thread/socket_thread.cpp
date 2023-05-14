//
// Created by TSU on 2023-05-07.
//

#include "std_common.hpp"
#include "thread/socket_thread.hpp"
#include "net/abstract_session.hpp"

hl::socket_thread::socket_thread(uint32_t id)
    : _id(id)
    , _cv()
    , _mutex()
    , _alive(true)
    , _queued_sessions()
    , _thread()
{
    LOGD << "Constructed socket thread - " << _id;
    std::thread th([this](){ run(); });
    _thread = std::move(th);
}

hl::socket_thread::~socket_thread()
{
    LOGD << "Destructed socket thread - " << _id;
}

void hl::socket_thread::run()
{
    while (_alive)
    {
        std::shared_ptr<abstract_session> session;
        synchronized (_mutex)
        {
            _cv.wait(_ul, [this] { return !_queued_sessions.empty() || !_alive; });
            if (_alive && _queued_sessions.empty())
                break;
            session = std::move(_queued_sessions.front());
            _queued_sessions.pop_front();
            LOGV << session << "was dequeued from socket thread - " << _id;
        }
        session->do_socket_op();
    }
    LOGV << "socket thread - " << _id << " has finished.";
}

void hl::socket_thread::enqueue(const std::shared_ptr<abstract_session>& session)
{
    synchronized (_mutex)
    {
        _queued_sessions.push_back(session);
        LOGV << session << "was enqueued to socket thread - " << _id;
    }
    _cv.notify_one();
}

void hl::socket_thread::stop()
{
    _alive = false;
    _cv.notify_all();
    LOGV << "Requested stopping socket thread - " << _id;
    _thread.join();
    LOGI << "Stopped socket thread - " << _id;
}
