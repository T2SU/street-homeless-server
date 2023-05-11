//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_CONFIG_HPP
#define STREET_HOMELESS_SERVER_CONFIG_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::database
{
    class config
    {
    private:
        std::shared_ptr<sqlpp::mysql::connection_config> _config;
        size_t _pooling_min;
        size_t _pooling_max;

        void load_config();

    public:
        config();

        [[nodiscard]] std::shared_ptr<sqlpp::mysql::connection_config> get() const;
        [[nodiscard]] std::shared_ptr<sqlpp::mysql::connection_config> operator()() const;
        [[nodiscard]] size_t get_pooling_min() const;
        [[nodiscard]] size_t get_pooling_max() const;
    };
}

#endif //STREET_HOMELESS_SERVER_CONFIG_HPP
