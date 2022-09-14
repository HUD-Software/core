#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_ASSIGNABLE_H
#define HD_INC_CORE_TRAITS_IS_ASSIGNABLE_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether from_t is a type assignable to to_t. (declval<to_t>() = declval<from_t>()); is well-formed). */
    template<typename to_t, typename from_t>
    struct is_assignable
        : bool_constant<__is_assignable(to_t, from_t)> {
    };

    /** Equivalent of is_assignable<to_t, from_t>::value. */
    template<typename to_t, typename from_t>
    inline constexpr bool is_assignable_v = is_assignable<to_t, from_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_ASSIGNABLE_H