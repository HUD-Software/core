#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_SAME_H
#define HD_INC_CORE_TRAITS_IS_SAME_H
#include "integral_constant.h"

namespace hud {

    /**
    * Checks whether type_t is the same type as u_type_t, including having the same const and/or volatile qualification, if any.
    * Two different type names are considered to represent the same type if -and only if- one is a typedef of the other:
    * Two names representing types with the exact same characteristics but which none is a typedef of the other are not considered the same type.
    */
    template<typename type_t, typename u_type_t>
    struct is_same
        : hud::false_type {
    };
    template<typename type_t>
    struct hud::is_same<type_t, type_t>
        : hud::true_type {
    };

    /** Equivalent of hud::is_same<type_t,u_type_t>::value. */
    template<typename type_t, typename u_type_t>
    inline constexpr bool is_same_v = hud::is_same<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_SAME_H