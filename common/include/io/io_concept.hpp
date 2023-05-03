//
// Created by TSU on 2023-05-03.
//

#ifndef STREET_HOMELESS_SERVER_IO_CONCEPT_HPP
#define STREET_HOMELESS_SERVER_IO_CONCEPT_HPP

template<typename Ty>
concept NetworkSize = sizeof(Ty) == 1;

#endif //STREET_HOMELESS_SERVER_IO_CONCEPT_HPP
