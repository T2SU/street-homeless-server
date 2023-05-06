//
// Created by TSU on 2023-05-07.
//

#include "std_common.hpp"
#include "thread/socket_thread.hpp"
#include "net/abstract_session.hpp"
#include "io/in_buffer.hpp"

socket_thread::socket_thread(uint32_t id)
    : _id(id)
    , _cv()
    , _mutex()
    , _alive(true)
    , _jobs()
{
    LOGD << "Constructed socket thread - " << _id;
}

socket_thread::~socket_thread()
{
    LOGD << "Destructed socket thread - " << _id;
}

void socket_thread::run(socket_thread *$this)
{
    while ($this->_alive)
    {
        while (true)
        {
            socket_job* job;
            synchronized ($this->_mutex)
            {
                if (!$this->_jobs.empty())
                    job = &$this->_jobs.front();
                else
                    job = nullptr;
            }
            if (job == nullptr)
                break;
            job->socket->on_packet(*(job->packet));
            $this->_jobs.pop_front();
        }
        std::unique_lock<decltype($this->_mutex)> ul($this->_mutex);
        $this->_cv.wait(ul);
    }
}

void socket_thread::enqueue(socket_thread::socket_job job)
{
    synchronized (_mutex)
    {
        _jobs.emplace_back(job);
        _cv.notify_one();
    }
}

void socket_thread::stop()
{
    _alive = false;
    LOGD << "Requested stopping socket thread - " << _id;
}
