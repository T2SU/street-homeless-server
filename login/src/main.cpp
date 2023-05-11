//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_server.hpp"

static void init_logger()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

    const auto log_config = hl::yaml::load("log.yaml");

    plog::Severity severity;
    const auto severity_str = log_config["max_severity"].as<std::string>("info");
    if (severity_str == "none") severity = plog::Severity::none;
    if (severity_str == "fatal") severity = plog::Severity::fatal;
    if (severity_str == "error") severity = plog::Severity::error;
    if (severity_str == "warning") severity = plog::Severity::warning;
    if (severity_str == "info") severity = plog::Severity::info;
    if (severity_str == "debug") severity = plog::Severity::debug;
    if (severity_str == "verbose") severity = plog::Severity::verbose;
    plog::init(severity, &consoleAppender);
}

static void start_server()
{
    const auto common_config = hl::yaml::load("common.yaml");
    const auto login_config = hl::yaml::load("login.yaml");

    const auto threads = common_config["thread"]["socket_thread_count"].as<int32_t>(4);
    hl::singleton<hl::socket_thread_pool>::get().begin(threads);

    const auto bind_address = login_config["network"]["address"].as<std::string>("0.0.0.0");
    const auto port = login_config["network"]["port"].as<int32_t>(7675);
    hl::singleton<hl::login::login_server>::get().begin(bind_address, port);

    LOGI << "Stopped event loop..";
}

int main()
{
    init_logger();
    start_server();
    return 0;
}