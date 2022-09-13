#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_ASSIGNABLE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether U is a type assignable to T, and such operation is known not to throw any exception. */
    template <typename T, typename U = T>
    struct IsNothrowAssignable
        : BoolConstant<__is_nothrow_assignable(T, U)> {
    };

    /** Equivalent of IsNothrowAssignable<T,U=T>::Value. */
    template <typename T, typename U = T>
    inline constexpr bool IsNothrowAssignableV = IsNothrowAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NO_THROW_ASSIGNABLE_H