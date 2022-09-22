#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_UNION_H
#define HD_INC_CORE_TRAITS_IS_UNION_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether type_t is a union type. */
    template<typename type_t>
    struct is_union
        : hud::bool_constant<__is_union(type_t)> {
    };

    /** Equivalent of is_union<type_t>::value */
    template<typename type_t>
    inline constexpr bool is_union_v = is_union<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_UNION_H