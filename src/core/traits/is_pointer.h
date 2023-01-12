#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_POINTER_H
#define HD_INC_CORE_TRAITS_IS_POINTER_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a pointer to object or a pointer to function (but not a pointer to member/member function). */
    template <typename type_t>
    struct is_pointer
        : hud::false_type
    {
    };
    template <typename type_t>
    struct is_pointer<type_t *>
        : hud::true_type
    {
    };

    /** Equivalent of is_pointer<type_t>::value. */
    template <typename type_t>
    inline constexpr bool is_pointer_v = is_pointer<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_POINTER_H