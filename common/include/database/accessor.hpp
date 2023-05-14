//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_ACCESSOR_HPP
#define STREET_HOMELESS_SERVER_ACCESSOR_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "job.hpp"

namespace hl::database
{
    class accessor
    {
    private:
        std::mutex _mutex;
        std::condition_variable _cv;
        std::queue<std::shared_ptr<job>> _waiting_jobs;
        std::atomic_bool _stop;
        std::thread _thread;
        std::vector<std::shared_ptr<job>> _jobs;


    public:
        accessor();
        ~accessor();

        template<typename JobTy, typename... Types>
        void post(Types&&... val)
        {
            synchronized (_mutex)
            {
                _waiting_jobs.push(std::static_pointer_cast<job>(std::make_shared<JobTy>(val...)));
            }
            _cv.notify_one();
        }

        void run();
        void end();
    };
}



#endif //STREET_HOMELESS_SERVER_ACCESSOR_HPP
