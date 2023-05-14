//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_E9FC8A7F0BAC44D0A3FB47C19847065F
#define STREET_HOMELESS_SERVER_E9FC8A7F0BAC44D0A3FB47C19847065F

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::game
{
    class item
    {
    private:
        uint64_t _sn;
        int32_t _item_id;
        int32_t _amount;

    public:
        item(int32_t item_id, int32_t amount);

        uint64_t get_sn() const;
        void set_sn(uint64_t sn);
        int32_t get_item_id() const;
        void set_item_id(int32_t item_id);
        int32_t get_amount() const;
        void set_amount(int32_t amount);
    };

    std::ostream& operator<<(std::ostream& os, const item& item);
}

#endif //STREET_HOMELESS_SERVER_E9FC8A7F0BAC44D0A3FB47C19847065F
