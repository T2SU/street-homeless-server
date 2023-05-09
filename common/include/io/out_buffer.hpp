//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_OUT_BUFFER_HPP
#define STREET_HOMELESS_SERVER_OUT_BUFFER_HPP

#include "io/io_concept.hpp"
#include "net/message.hpp"
#include "net/network_exceptions.hpp"
#include "utils/net_tools.hpp"

template<typename Ty> requires NetworkSize<Ty>
class basic_out_buffer
{
private:
    std::vector<Ty> _buffer;

public:
    basic_out_buffer() = default;
    explicit basic_out_buffer(size_t size) : _buffer() { _buffer.reserve(size); }
    virtual ~basic_out_buffer() = default;

    [[nodiscard]]
    const Ty* get_buffer() const
    {
        return _buffer.data();
    }

    [[nodiscard]]
    size_t get_buffer_size() const
    {
        return _buffer.size();
    }

    template<typename T>
    void write(const T& data)
    {
        std::array<Ty, sizeof(T)> tmp = std::bit_cast<decltype(tmp)>(data);
        _buffer.insert(_buffer.end(), tmp.begin(), tmp.end());
    }

    void write_str(const std::string& str)
    {
        write(static_cast<uint16_t>(str.length()));
        write(str.c_str(), 0, str.length());
    }

    void write(const void* buffer, size_t offset, size_t len)
    {
        const Ty* const ori_buffer = reinterpret_cast<const Ty*>(buffer);
        _buffer.insert(_buffer.end(), &ori_buffer[offset], &ori_buffer[len]);
    }

    [[nodiscard]]
    std::string dump_packet() const
    {
        return hl::net_tools::dump_memory(get_buffer(), get_buffer_size());
    }
};

class out_buffer : public basic_out_buffer<std::byte>
{
public:
    out_buffer() : basic_out_buffer() {}
    explicit out_buffer(svr_msg msg, size_t size = 128) : basic_out_buffer(size)
    {
        write(msg);
    }
    ~out_buffer() override = default;
};

#endif //STREET_HOMELESS_SERVER_OUT_BUFFER_HPP
