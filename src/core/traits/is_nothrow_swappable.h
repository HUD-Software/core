#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_SWAPPABLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_SWAPPABLE_H
#include "integral_constant.h"
#include "is_swappable.h"
#include "conjunction.h"

namespace hud
{

    namespace details
    {
        template <typename type_t, typename u_type_t = type_t>
        struct is_nothrow_swappable_impl
            : hud::bool_constant<noexcept(swap(declval<type_t>(), declval<u_type_t>())) && noexcept(swap(declval<type_t>(), declval<u_type_t>()))>
        {
        };
    }

    /** Check whether type_t is swappable with u_type_t and such swap is known not to throw any exception. */
    template <typename type_t, typename u_type_t = type_t>
    struct is_nothrow_swappable
        : hud::conjunction<is_swappable<type_t, u_type_t>, details::is_nothrow_swappable_impl<hud::add_lvalue_reference_t<type_t>, hud::add_lvalue_reference_t<u_type_t>>>
    {
    };

    /** Equivalent of is_nothrow_swappable<type_t, u_type_t>::value. */
    template <typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOTHROW_SWAPPABLE_H