#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether u_type_t is a type trivially assignable to type_t. (declval<type_t>() = declval<u_type_t>() is well-formed) */
    template <typename type_t, typename u_type_t>
    struct is_trivially_assignable
        : hud::bool_constant<__is_trivially_assignable(type_t, u_type_t)>
    {
    };

    /** Equivalent of is_trivially_assignable<type_t, u_type_t>::value. */
    template <typename type_t, typename u_type_t>
    inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H