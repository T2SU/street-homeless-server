//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_IO_CONCEPT_HPP
#define STREET_HOMELESS_SERVER_IO_CONCEPT_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

template<typename Ty>
concept NetworkSize = sizeof(Ty) == 1;

#endif //STREET_HOMELESS_SERVER_IO_CONCEPT_HPP
