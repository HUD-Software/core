#ifndef HD_INC_CORE_TYPES_H
#define HD_INC_CORE_TYPES_H

#include "types/types.h"

#define define_integer_literals(type)                                   \
    constexpr type operator""_##type(unsigned long long value) noexcept \
    {                                                                   \
        return static_cast<type>(value);                                \
    }                                                                   \
    constexpr type operator""_##type(long double value) noexcept        \
    {                                                                   \
        return static_cast<type>(value);                                \
    }
define_integer_literals(i64)

#endif // HD_INC_CORE_TYPES_H