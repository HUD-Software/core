#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_ASSIGNABLE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether From is a type assignable to To. (declval<To>() = declval<From>()); is well-formed). */
    template<typename To, typename From>
    struct IsAssignable
        : BoolConstant<__is_assignable(To, From)> {
    };

    /** Equivalent of IsAssignable<To, From>::Value. */
    template<typename To, typename From>
    inline constexpr bool IsAssignableV = IsAssignable<To, From>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_ASSIGNABLE_H