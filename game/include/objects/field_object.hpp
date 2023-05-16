//
// Created by T2SU on 2023/05/16.
//

#ifndef STREET_HOMELESS_SERVER_D9008AC0DCF84D37BB8062C8B2EB2AFE
#define STREET_HOMELESS_SERVER_D9008AC0DCF84D37BB8062C8B2EB2AFE

#include "io/out_buffer.hpp"

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif
namespace hl::game
{
    // TODO object pooling

    class field_object
    {
    protected:
        uint64_t _object_id;

    public:
        explicit field_object(uint64_t object_id);
        virtual ~field_object();

        uint64_t get_id() const;

        virtual void encode_enter(out_buffer& out_buffer) const = 0;
        virtual void encode_leave(out_buffer& out_buffer) const = 0;

        virtual std::string to_string() const;
    };

} // game

#endif //STREET_HOMELESS_SERVER_D9008AC0DCF84D37BB8062C8B2EB2AFE
