#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_COMPARABLE_WITH_GREATER_EQUAL_H
#define HD_INC_CORE_TRAITS_IS_COMPARABLE_WITH_GREATER_EQUAL_H
#include "integral_constant.h"
#include "void_t.h"
#include "../templates/declval.h"
#include "add_const.h"
#include "add_lvalue_reference.h"

namespace hud {

    /** Checks whether lhs_t type is comparable with rhs_t type with operator>=. */
    template<typename lhs_t, typename rhs_t, typename = void>
    struct is_comparable_with_greater_equal
        : hud::false_type {
    };

    template<typename lhs_t, typename rhs_t>
    struct is_comparable_with_greater_equal<lhs_t, rhs_t, void_t<decltype(hud::declval<add_const_t<add_lvalue_reference_t<lhs_t>>>() >= hud::declval<add_const_t<add_lvalue_reference_t<rhs_t>>>())>>
        : hud::true_type {
    };

    /** Equivalent of is_comparable_with_greater_equal<lhs_t,rhs_t>::value. */
    template<typename lhs_t, typename rhs_t>
    inline constexpr bool is_comparable_with_greater_equal_v = is_comparable_with_greater_equal<lhs_t, rhs_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_COMPARABLE_WITH_GREATER_EQUAL_H