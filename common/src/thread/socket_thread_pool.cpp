//
// Created by TSU on 2023-05-08.
//

#include "std_common.hpp"
#include "thread/socket_thread_pool.hpp"

homeless::socket_thread_pool::socket_thread_pool()
    : _threads()
    , _idx(0) {}

void homeless::socket_thread_pool::begin(uint32_t threads)
{
    for (auto i = 0u; i < threads; ++i)
        _threads.emplace_back(std::make_shared<homeless::socket_thread>(i));
    for (const auto& thread_ptr : _threads)
    {
        std::thread th(homeless::socket_thread::run, thread_ptr.get());
        th.detach();
    }
    LOGD << "Started " << _threads.size() << " socket threads";
}

void homeless::socket_thread_pool::end()
{
    for (const auto& thread_ptr : _threads)
    {
        thread_ptr.get()->stop();
    }
    LOGD << "Requested stopping " << _threads.size() << " socket threads";
}

std::shared_ptr<homeless::socket_thread> homeless::socket_thread_pool::get_worker_thread() const
{
    return _threads[(_idx++) % _threads.size()];
}