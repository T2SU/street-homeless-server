//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_IN_BUFFER_HPP
#define STREET_HOMELESS_SERVER_IN_BUFFER_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "io/io_concept.hpp"
#include "net/network_exceptions.hpp"
#include "utils/net_tools.hpp"

/* https://stackoverflow.com/a/13059195 (Dietmar Kühl) (CC BY-SA 3.0) */
struct membuf: std::streambuf {
    membuf(char const* base, size_t size) {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }
};
struct imemstream: virtual membuf, std::istream {
    imemstream(char const* base, size_t size)
            : membuf(base, size)
            , std::istream(static_cast<std::streambuf*>(this)) {
    }
};

template<typename Ty> requires NetworkSize<Ty>
class basic_in_buffer {
private:
    std::vector<Ty> _buffer;
    size_t _offset;

public:

    basic_in_buffer()
        : _buffer()
        , _offset(0) {}

    basic_in_buffer(const void* buffer, size_t len)
        : _buffer()
        , _offset(0)
    {
        append_buffer(buffer, len);
    }

    basic_in_buffer& operator= (const basic_in_buffer&) = delete;
    basic_in_buffer& operator= (basic_in_buffer&&) = delete;

    template<typename T>
    T read()
    {
        if (_offset + sizeof(T) > get_raw_buffer_size())
            throw end_of_stream_exception();
        std::array<Ty, sizeof(T)> tmp;
        std::memcpy(tmp.data(), &_buffer[_offset], sizeof(T));
        _offset += sizeof(T);
        return std::bit_cast<T>(tmp);
    }

    void append_buffer(const void* buffer, size_t len)
    {
        const Ty* const ori_buffer = reinterpret_cast<const Ty*>(buffer);
        _buffer.insert(_buffer.end(), &ori_buffer[0], &ori_buffer[len]);
    }

    std::string read_str()
    {
        const auto len = read<uint16_t>();
        if (_offset + len > get_raw_buffer_size())
            throw end_of_stream_exception();
        if (len == 0)
            return "";
        auto ret = std::string(reinterpret_cast<const char*>(&_buffer[_offset]), static_cast<size_t>(len));
        _offset += len;
        return ret;
    }

    void read(void* dst, size_t len)
    {
        if (_offset + len > get_raw_buffer_size())
            throw end_of_stream_exception();
        std::memcpy(dst, &_buffer[_offset], len);
        _offset += len;
    }

    template<typename T>
    void read_pb(T& protobuf)
    {
        const auto len = read<uint16_t>();
        if (_offset + len > get_raw_buffer_size())
            throw end_of_stream_exception();
        protobuf.ParseFromArray(&_buffer[_offset], len);
        _offset += len;
    }

    [[nodiscard]]
    std::string dump_packet() const
    {
        return hl::net_tools::dump_memory(get_raw_buffer(), get_raw_buffer_size());
    }

    [[nodiscard]]
    const Ty* get_raw_buffer() const
    {
        return _buffer.data();
    }

    [[nodiscard]]
    size_t get_raw_buffer_size() const
    {
        return _buffer.size();
    }

    [[nodiscard]]
    size_t remaining() const
    {
        if (_buffer.size() < _offset) return 0;
        return _buffer.size() - _offset;
    }
};

class in_buffer : public basic_in_buffer<std::byte>
{
public:
    in_buffer() : basic_in_buffer() {}
    in_buffer(const void *buffer, size_t len) : basic_in_buffer(buffer, len) {}
};

#endif //STREET_HOMELESS_SERVER_IN_BUFFER_HPP
