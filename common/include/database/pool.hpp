//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_POOL_HPP
#define STREET_HOMELESS_SERVER_POOL_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "config.hpp"

namespace hl::database
{
    template<typename Conn>
    class connection_pool;

    template<typename Conn>
    class proxy_connection final
    {
    private:
        friend class connection_pool<Conn>;

        proxy_connection()
                : _pool(), _conn()
        {}

        proxy_connection(connection_pool<Conn> *pool, Conn *conn) noexcept
                : _pool(pool), _conn(conn)
        {
            LOGV << "created pool connection";
        }

        connection_pool<Conn> *_pool;
        Conn *_conn;

    public:
        proxy_connection(proxy_connection &&other) noexcept
                : _pool(other._pool), _conn(other._conn)
        {
        }

        proxy_connection &operator=(proxy_connection &&other) noexcept
        {
            if (this == &other)
                return *this;
            _pool = other._pool;
            _conn = other._conn;
            return *this;
        }

        ~proxy_connection()
        {
            LOGV << "releasing pool connection";
            _pool->release(*this);
        }

        Conn *operator->()
        { return _conn; }

        Conn &operator*()
        { return *_conn; }
    };

    template<typename Conn>
    class connection_pool final
    {
    private:
        mutable std::mutex _mutex;
        Conn *_connections;
        const size_t _min;
        const size_t _max;
        std::allocator<Conn> _allocator;
        size_t _current;
        std::queue<Conn *> _idle_connections;

        friend class proxy_connection<Conn>;

        void connect(Conn *conn)
        {
            std::allocator_traits<std::allocator<Conn>>::construct(_allocator, conn, hl::singleton<hl::database::config>::get().get());
            LOGV << "connected to db successfully!";
        }

        void release(proxy_connection<Conn> &conn)
        {
            synchronized (_mutex)
            {
                _idle_connections.push(conn._conn);
            }
        }

        Conn *get_next_place()
        {
            if (_current >= _max) return nullptr;
            return &_connections[_current++];
        }

    public:
        connection_pool(size_t min, size_t max)
                : _mutex()
                , _connections()
                , _min(min)
                , _max(max)
                , _allocator()
                , _current()
                , _idle_connections()
        {
            _connections = _allocator.allocate(max);
            LOGV << "Allocated [" << min << " ~ " << max << "] size connection pool.";

            for (size_t i = 0; i < _min; i++)
            {
                connect(&_connections[i]);
                synchronized (_mutex)
                {
                    _idle_connections.push(&_connections[i]);
                }
            }
            _current = _min;
        }

        ~connection_pool()
        {
            for (auto i = 0; i < _current; i++)
            {
                std::allocator_traits<std::allocator<Conn>>::destroy(_allocator, &_connections[i]);
            }
            LOGV << "Destroyed " << _current << " connections";

            _allocator.deallocate(_connections, _max);
            LOGV << "Deallocated " << _max << " size connection pool.";
        }

        proxy_connection<Conn> get_connection()
        {
            Conn *conn_ptr;
            bool need_init;
            synchronized (_mutex)
            {
                if (!_idle_connections.empty())
                {
                    conn_ptr = _idle_connections.front();
                    _idle_connections.pop();
                    need_init = false;
                } else
                {
                    conn_ptr = get_next_place();
                    need_init = true;
                }
            }
            if (conn_ptr == nullptr)
            {
                LOGF << "No available db connection.";
                throw std::runtime_error("No available db connection.");
            }
            if (need_init)
                connect(conn_ptr);
            return proxy_connection<Conn>(this, conn_ptr);
        }
    };
}

namespace hl
{
    template<>
    inline hl::database::connection_pool<sqlpp::mysql::connection>&
            singleton<hl::database::connection_pool<sqlpp::mysql::connection>>::get()
    {
        static hl::database::connection_pool<sqlpp::mysql::connection> _pool(
                singleton<hl::database::config>::get().get_pooling_min(),
                singleton<hl::database::config>::get().get_pooling_max());
        return _pool;
    }
}

#endif //STREET_HOMELESS_SERVER_POOL_HPP
