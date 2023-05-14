//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/game_server.hpp"
#include "net/master.hpp"

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

static void start_server(const char* config)
{
    const auto common_config = hl::yaml::load("common.yaml");
    const auto game_config = hl::yaml::load(std::format("{}.yaml", config));

    const auto threads = common_config["thread"]["socket_thread_count"].as<int32_t>(4);
    hl::singleton<hl::socket_thread_pool>::get().begin(threads);

    const auto idx = game_config["config"]["game"]["index"].as<uint32_t>();
    const auto flags = game_config["config"]["game"]["flags"];
    const auto flag = std::accumulate(flags.begin(), flags.end(), game_server_flag::empty,
                                      [](game_server_flag f, const auto& n){
        const auto type = n.template as<std::string>();
        if (type == "field")
            return static_cast<game_server_flag>(f | game_server_flag::game_field);
        if (type == "instance")
            return static_cast<game_server_flag>(f | game_server_flag::game_instance);
        throw std::invalid_argument(std::format("invalid game server flag type '{}'", type));
    });
    hl::singleton<hl::game::game_server>::get().set_server_config(idx, flag);

    const auto endpoint_address = game_config["network"][config]["endpoint"]["address"].as<std::string>();
    const auto endpoint_port = game_config["network"][config]["endpoint"]["port"].as<uint16_t>();
    hl::singleton<hl::game::game_server>::get().set_endpoint(endpoint_address, endpoint_port);

    const auto master_address = game_config["network"]["inter_server"]["master_host"].as<std::string>();
    const auto master_port = game_config["network"]["inter_server"]["master_port"].as<int32_t>();
    hl::singleton<hl::connector<hl::game::master>>::get().connect(master_address, master_port);

    const auto bind_address = game_config["network"]["address"].as<std::string>("0.0.0.0");
    const auto port = game_config["network"]["port"].as<int32_t>(7676);
    hl::singleton<hl::game::game_server>::get().begin(bind_address, port);
    LOGI << "Stopped event loop..";
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cerr << "Usage:";
        std::cerr << "  " << argv[0] << " {config name}" << std::endl;
        return 1;
    }

    init_logger();
    start_server(argv[1]);
    return 0;
}