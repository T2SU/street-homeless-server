//
// Created by T2SU on 2023/05/16.
//

#ifndef STREET_HOMELESS_SERVER_0B64770214D54C11850242972DC80943
#define STREET_HOMELESS_SERVER_0B64770214D54C11850242972DC80943

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::game
{
    class portal
    {
    private:
        const uint32_t _sn;
        const std::string _name;
        const vector3 _pos;
        const quaternion _rot;

    public:
        portal(uint32_t sn, const std::string &name, const vector3 &pos, const quaternion &rot);

        [[nodiscard]] uint32_t get_sn() const;
        [[nodiscard]] const std::string &get_name() const;
        [[nodiscard]] const vector3 &get_pos() const;
        [[nodiscard]] const quaternion &get_rot() const;
    };
}

#endif //STREET_HOMELESS_SERVER_0B64770214D54C11850242972DC80943
