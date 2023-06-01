//
// Created by TSU on 2023-06-01.
//

#include "std_common.hpp"
#include "resources/region_info.hpp"

namespace hl
{
    region_info::region_info(region_id_t id)
        : abstract_data("regions/" + std::to_string(id))
    {
    }

    const region_info &region_info_s::get(region_id_t region_id)
    {
        synchronized (_mutex)
        {
            const auto it = _infos.find(region_id);
            if (it != _infos.end())
                return *(it->second);
        }
        std::unique_ptr<region_info> info = std::make_unique<region_info>(region_id);
        synchronized (_mutex)
        {
            const auto it = _infos.try_emplace(region_id, std::move(info));
            return *(it.first->second);
        }

        throw std::runtime_error("not expected running point");
    }
} // hl