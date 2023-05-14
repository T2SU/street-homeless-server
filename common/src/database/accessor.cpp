//
// Created by TSU on 2023-05-11.
//

#include "std_common.hpp"
#include "database/accessor.hpp"
#include "database/pool.hpp"

using pool_type = hl::database::connection_pool<sqlpp::mysql::connection>;

hl::database::accessor::accessor()
    : _mutex()
    , _cv()
    , _waiting_jobs()
    , _stop()
    , _thread()
    , _jobs()
{
    _jobs.reserve(64);
    // touch connection pool first
    hl::singleton<pool_type>::get();

    std::thread worker([](hl::database::accessor *$this) { $this->run(); }, this);
    _thread = std::move(worker);

    LOGI << "Started database accessor.";
}

hl::database::accessor::~accessor()
{
    end();
}

void hl::database::accessor::run()
{
    while (true)
    {
        synchronized (_mutex)
        {
            _cv.wait(_ul, [this]{ return _stop || !_waiting_jobs.empty(); });
            if (_stop || _waiting_jobs.empty())
                return;
            while (!_waiting_jobs.empty())
            {
                _jobs.push_back(_waiting_jobs.front());
                _waiting_jobs.pop();
            }
        }

        auto conn = hl::singleton<pool_type>::get().get_connection();
        for (auto& job : _jobs)
        {
            job->process(*conn);
        }
        _jobs.clear();
    }
}

void hl::database::accessor::end()
{
    if (_stop) return;
    LOGD << "Stopping database accessor..";
    _stop = true;
    _cv.notify_all();
    _thread.join();
    LOGI << "Stopped database accessor.";
}
