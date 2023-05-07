//
// Created by TSU on 2023-05-07.
//

#include "std_common.hpp"
#include "thread/socket_thread.hpp"
#include "net/abstract_session.hpp"

homeless::socket_thread::socket_thread(uint32_t id)
    : _id(id)
    , _cv()
    , _mutex()
    , _alive(true)
    , _queued_sessions()
{
    LOGD << "Constructed socket thread - " << _id;
}

homeless::socket_thread::~socket_thread()
{
    LOGD << "Destructed socket thread - " << _id;
}

void homeless::socket_thread::run(socket_thread *$this)
{
    while ($this->_alive)
    {
        abstract_session* session;
        synchronized ($this->_mutex)
        {
            $this->_cv.wait(_ul, [$this]{ return !$this->_queued_sessions.empty() || !$this->_alive; });
            if ($this->_alive && $this->_queued_sessions.empty())
                break;
            session = $this->_queued_sessions.front();
            $this->_queued_sessions.pop_front();
            LOGV << "socket " << session->get_id() << " was dequeued from socket thread - " << $this->_id;
        }
        session->do_socket_op();
    }
    LOGD << "socket thread - " << $this->_id << " has finished.";
}

void homeless::socket_thread::enqueue(abstract_session* session)
{
    synchronized (_mutex)
    {
        _queued_sessions.push_back(session);
        LOGV << "socket " << session->get_id() << " was enqueued to socket thread - " << _id;
    }
    _cv.notify_one();
}

void homeless::socket_thread::stop()
{
    _alive = false;
    _cv.notify_all();
    LOGD << "Requested stopping socket thread - " << _id;
}
