//
// Created by TSU on 2023-06-01.
//

#ifndef STREET_HOMELESS_SERVER_E6B7207819544DD0B757364BE95B9C35
#define STREET_HOMELESS_SERVER_E6B7207819544DD0B757364BE95B9C35

#include "region.hpp"

namespace hl::game
{
    class region_man
    {
    private:
        std::unordered_map<region_sn_t, std::unique_ptr<region>> _regions;
        mutable std::mutex _mutex;

    public:
        region_man();

        void create(region_sn_t sn, region_id_t id);
        region* get(region_sn_t sn) const;
        void remove(region_sn_t sn);
    };
}

#endif //STREET_HOMELESS_SERVER_E6B7207819544DD0B757364BE95B9C35
