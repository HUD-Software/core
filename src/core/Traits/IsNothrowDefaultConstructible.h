#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a default constructible type, and such construction is known not to throw any exception. */
    template <typename T>
    struct IsNothrowDefaultConstructible
        : BoolConstant<__is_nothrow_constructible(T)> {
    };

    /** Equivalent of IsNothrowDefaultConstructible<T>::Value. */
    template <typename T>
    inline constexpr bool IsNothrowDefaultConstructibleV = IsNothrowDefaultConstructible<T>::Value;





} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_H