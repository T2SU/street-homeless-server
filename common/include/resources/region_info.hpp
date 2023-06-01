//
// Created by TSU on 2023-06-01.
//

#ifndef STREET_HOMELESS_SERVER_FC94EE35B2734461A7B241FDDF18AF44
#define STREET_HOMELESS_SERVER_FC94EE35B2734461A7B241FDDF18AF44

#include "abstract_data.hpp"

namespace hl
{
    class region_info : public abstract_data
    {
    public:
        explicit region_info(region_id_t id);

        [[nodiscard]] inline const auto get_region_id() const { return _data["region_id"].as<region_id_t>(); }
        [[nodiscard]] inline const auto get_region_type() const
        {
            const auto t = _data["region_type"].as<uint32_t>();
            switch (t)
            {
                case 0: return region_type::regular;
                case 1: return region_type::instance;
                default: throw std::runtime_error(fmt::format("unknown region type {}", t));
            }
        }
        [[nodiscard]] inline const auto get_max_players() const { return _data["max_players"].as<size_t>(); }
    };

    class region_info_s
    {
    private:
        std::mutex _mutex;
        std::unordered_map<region_id_t, std::unique_ptr<region_info>> _infos;

    public:
        inline region_info_s(): _mutex(), _infos() {}

        const region_info& get(region_id_t region_id);
    };
}

#endif //STREET_HOMELESS_SERVER_FC94EE35B2734461A7B241FDDF18AF44
