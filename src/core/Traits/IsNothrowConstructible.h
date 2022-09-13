#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a constructible type using the set of argument types specified by TArgs, and such construction is known not to throw any exception. */
    template <typename T, typename... TArgs>
    struct IsNothrowConstructible
        : BoolConstant<__is_nothrow_constructible(T, TArgs...)> {
    };

    /** Equivalent of IsNothrowConstructible<T, TArgs...>::Value. */
    template <typename T, typename... TArgs>
    inline constexpr bool IsNothrowConstructibleV = IsNothrowConstructible<T, TArgs...>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H