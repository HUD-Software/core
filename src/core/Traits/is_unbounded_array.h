#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_UNBOUNDED_ARRAY_H
#define HD_INC_CORE_TRAITS_IS_UNBOUNDED_ARRAY_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether type_t is a unbounded array type. */
    template<typename type_t>
    struct is_unbounded_array 
        : false_type {
    };
    template<typename type_t>
    struct is_unbounded_array<type_t[]>
        : true_type {
    };

    /** Equivalent of is_unbounded_array<type_t>::value. */
    template <typename type_t>
    inline constexpr bool is_unbounded_array_v = is_unbounded_array<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_UNBOUNDED_ARRAY_H