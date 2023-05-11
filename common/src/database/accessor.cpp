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
{
    // touch connection pool first
    hl::singleton<pool_type>::get();

    std::thread worker([](hl::database::accessor *$this) { $this->run(); }, this);
    worker.detach();
    LOGI << "Started database accessor.";
}

void hl::database::accessor::run()
{
    std::vector<std::shared_ptr<job>> jobs;
    jobs.reserve(64);

    while (true)
    {
        synchronized (_mutex)
        {
            _cv.wait(_ul, [this]{ return _stop || !_waiting_jobs.empty(); });
            if (_stop || _waiting_jobs.empty())
                return;
            while (!_waiting_jobs.empty())
            {
                jobs.push_back(_waiting_jobs.front());
                _waiting_jobs.pop();
            }
        }

        auto conn = hl::singleton<pool_type>::get().get_connection();
        for (auto& job : jobs)
        {
            job->process(*conn);
        }
        jobs.clear();
    }
    LOGI << "Stopped database accessor.";
}

void hl::database::accessor::end()
{
    LOGI << "Stopping database accessor..";
    _stop = true;
    _cv.notify_all();
}
