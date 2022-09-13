#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_POWER_OF_TWO_H
#define HD_INC_OSLAYER_TRAITS_IS_POWER_OF_TWO_H
#include "IntegralConstant.h"

namespace hud {

    /** Check wheter the given number is a power of two or not. */
    template<usize value>
    struct IsPowerOfTwo
        : BoolConstant<value && !(value& (value - 1))> {
    };

    /** Equivalent of IsPowerOfTwo<value>::Value. */
    template<usize value>
    inline constexpr bool IsPowerOfTwoV = IsPowerOfTwo<value>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_POWER_OF_TWO_H