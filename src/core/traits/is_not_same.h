#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_NOT_SAME_H
#define HD_INC_CORE_TRAITS_IS_NOT_SAME_H
#include "negation.h"
#include "is_same.h"

namespace hud
{

    /**
     * Checks whether type_t is not the same type as u_type_t, including having the same const and/or volatile qualification, if any.
     * Two different type names are considered to represent the same type if -and only if- one is a typedef of the other:
     * Two names representing types with the exact same characteristics but which none is a typedef of the other are not considered the same type.
     */
    template <typename type_t, typename u_type_t>
    struct is_not_same
        : hud::negation<hud::is_same<type_t, u_type_t>>
    {
    };

    /** Equivalent of is_not_same<type_t, u_type_t>::value. */
    template <typename type_t, typename u_type_t>
    inline constexpr bool is_not_same_v = is_not_same<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOT_SAME_H