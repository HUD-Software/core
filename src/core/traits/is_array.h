#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_ARRAY_H
#define HD_INC_CORE_TRAITS_IS_ARRAY_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether type_t is an array type ( Bounded or unbounded ). */
    template<typename type_t>
    struct is_array
        : false_type {
    };
    template <typename type_t, usize extent>
    struct is_array<type_t[extent]>
        : true_type {
    };
    template <typename type_t>
    struct is_array<type_t[]>
        : true_type {
    };

    /** Equivalent of is_array<type_t>::value. */
    template <typename type_t>
    inline constexpr bool is_array_v = is_array<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_ARRAY_H
