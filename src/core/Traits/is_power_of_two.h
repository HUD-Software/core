#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_POWER_OF_TWO_H
#define HD_INC_CORE_TRAITS_IS_POWER_OF_TWO_H
#include "integral_constant.h"

namespace hud {

    /** Check wheter the given number is a power of two or not. */
    template<usize value>
    struct is_power_of_two
        : bool_constant<value && !(value& (value - 1))> {
    };

    /** Equivalent of is_power_of_two<value>::value. */
    template<usize value>
    inline constexpr bool is_power_of_two_v = is_power_of_two<value>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_POWER_OF_TWO_H