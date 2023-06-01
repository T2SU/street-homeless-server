//
// Created by TSU on 2023-06-01.
//

#ifndef STREET_HOMELESS_SERVER_7260684F00F54A39ACC56B2F8890D1E3
#define STREET_HOMELESS_SERVER_7260684F00F54A39ACC56B2F8890D1E3

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "abstract_data.hpp"

namespace hl
{
    class map_data : public abstract_data
    {
    public:
        explicit map_data(const std::string& scene);

        [[nodiscard]] inline const auto get_region_id() const { return _data["region_id"].as<region_id_t>(); }
    };

    class map_data_s
    {
    private:
        std::mutex _mutex;
        std::unordered_map<std::string, std::unique_ptr<map_data>> _infos;

    public:
        inline map_data_s(): _mutex(), _infos() {}

        const map_data& get(const std::string& scene);
    };
} // hl

#endif //STREET_HOMELESS_SERVER_7260684F00F54A39ACC56B2F8890D1E3
