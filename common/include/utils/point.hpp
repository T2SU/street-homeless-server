//
// Created by T2SU on 2023/05/14.
//

#ifndef STREET_HOMELESS_SERVER_2383AEAC473340A1B453BF0819031B9F
#define STREET_HOMELESS_SERVER_2383AEAC473340A1B453BF0819031B9F

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "protobuf.hpp"

namespace hl
{
    struct point
    {
        int16_t x;
        int16_t y;
    };
    struct vector3
    {
        float x;
        float y;
        float z;

        inline vector3() : x(), y(), z() {}
        inline vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        inline vector3(const pb::Vector3 v3)
            : x(v3.x())
            , y(v3.y())
            , z(v3.z()) {}

        inline operator pb::Vector3() const
        {
            pb::Vector3 v;
            v.set_x(x);
            v.set_y(y);
            v.set_z(z);
            return v;
        }
    };
    struct quaternion
    {
        float x;
        float y;
        float z;
        float w;

        inline quaternion() : x(), y(), z(), w() {}
        inline quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        inline quaternion(const pb::Quaternion q)
                : x(q.x())
                , y(q.y())
                , z(q.z())
                , w(q.w()){}

        inline operator pb::Quaternion() const
        {
            pb::Quaternion q;
            q.set_x(x);
            q.set_y(y);
            q.set_z(z);
            q.set_w(w);
            return q;
        }
    };
}



#endif //STREET_HOMELESS_SERVER_2383AEAC473340A1B453BF0819031B9F
