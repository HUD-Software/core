#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether U is a type trivially assignable to T. (declval<T>() = declval<U>() is well-formed) */
    template<typename T, typename U>
    struct IsTriviallyAssignable
        : BoolConstant<__is_trivially_assignable(T, U)> {
    };

    /** Equivalent of IsTriviallyAssignable<T, U>::Value. */
    template<typename T, typename U>
    inline constexpr bool IsTriviallyAssignableV = IsTriviallyAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_ASSIGNABLE_H