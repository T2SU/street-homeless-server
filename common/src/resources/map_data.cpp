//
// Created by TSU on 2023-06-01.
//

#include "std_common.hpp"
#include "resources/map_data.hpp"

namespace hl
{
    map_data::map_data(const std::string& scene)
        : abstract_data("maps/" + scene)
    {
    }

    const map_data &map_data_s::get(const std::string& scene)
    {
        synchronized (_mutex)
        {
            const auto it = _infos.find(scene);
            if (it != _infos.end())
                return *(it->second);
        }
        std::unique_ptr<map_data> info = std::make_unique<map_data>(scene);
        synchronized (_mutex)
        {
            const auto it = _infos.try_emplace(scene, std::move(info));
            return *(it.first->second);
        }

        throw std::runtime_error("not expected running point");
    }
} // hl