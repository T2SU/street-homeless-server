//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_NETWORK_EXCEPTIONS_HPP
#define STREET_HOMELESS_SERVER_NETWORK_EXCEPTIONS_HPP

#include "std_common.hpp"

class end_of_stream_exception : public std::runtime_error
{
public:
    end_of_stream_exception() : std::runtime_error("End of stream"){}
};

class acceptor_exception : public std::exception
{
private:
    const std::string _msg;
public:
    acceptor_exception(std::string operation, std::string bind_address, uint16_t bind_port)
        : _msg(std::format("failed to {}. {}:{}", operation, bind_address, bind_port)){}

        [[nodiscard]] const char* what() const noexcept override { return _msg.c_str(); }
};

#endif //STREET_HOMELESS_SERVER_NETWORK_EXCEPTIONS_HPP
