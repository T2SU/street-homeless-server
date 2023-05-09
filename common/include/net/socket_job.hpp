//
// Created by TSU on 2023-05-08.
//

#ifndef STREET_HOMELESS_SERVER_SOCKET_JOB_HPP
#define STREET_HOMELESS_SERVER_SOCKET_JOB_HPP

#include "io/in_buffer.hpp"
#include "io/out_buffer.hpp"

namespace hl
{
    enum class socket_op { READ, WRITE, CLOSE };

    class abstract_session;

    typedef struct socket_job
    {
        socket_op op;
        close_reason close_reason;
        std::shared_ptr<in_buffer> in_buffer;
        std::shared_ptr<out_buffer> out_buffer;
    } socket_job;

    typedef struct write_job
    {
        uv_write_t req;
        uv_buf_t buf;
        std::shared_ptr<out_buffer> out_buf;
        std::list<std::shared_ptr<struct write_job>>::iterator pointer;
    } write_job;
}

#endif //STREET_HOMELESS_SERVER_SOCKET_JOB_HPP
