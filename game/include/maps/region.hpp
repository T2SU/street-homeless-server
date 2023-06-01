//
// Created by TSU on 2023-06-01.
//

#ifndef STREET_HOMELESS_SERVER_245F932D6FF9409981D090C674F5C313
#define STREET_HOMELESS_SERVER_245F932D6FF9409981D090C674F5C313

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "map_factory.hpp"

namespace hl::game
{
    class region
    {
    private:
        const region_id_t _id;
        const region_sn_t _sn;

        map_factory _maps;

    public:
        region(region_id_t id, region_sn_t sn);

        inline auto& maps() { return _maps; }
    };
}


#endif //STREET_HOMELESS_SERVER_245F932D6FF9409981D090C674F5C313
