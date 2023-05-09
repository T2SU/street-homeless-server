#ifndef _SYNCHRONIZED_HPP_
#define _SYNCHRONIZED_HPP_

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#define synchronized(_mutex) for (auto _ul = std::unique_lock<decltype(_mutex)>(_mutex); _ul; _ul.unlock())

#endif