#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_ASSIGNABLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_ASSIGNABLE_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether u_type_t is a type assignable to type_t, and such operation is known not to throw any exception. */
    template <typename type_t, typename u_type_t = type_t>
    struct is_nothrow_assignable
        : hud::bool_constant<__is_nothrow_assignable(type_t, u_type_t)> {
    };

    /** Equivalent of is_nothrow_assignable<type_t,u_type_t=type_t>::value. */
    template <typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NO_THROW_ASSIGNABLE_H