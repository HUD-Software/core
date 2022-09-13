#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_SAME_H
#define HD_INC_OSLAYER_TRAITS_IS_SAME_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is the same type as U, including having the same const and/or volatile qualification, if any.
    * Two different type names are considered to represent the same type if -and only if- one is a typedef of the other:
    * Two names representing types with the exact same characteristics but which none is a typedef of the other are not considered the same type.
    */
    template<typename T, typename U>
    struct IsSame
        : FalseType {
    };
    template<typename T>
    struct IsSame<T, T>
        : TrueType {
    };

    /** Equivalent of IsSame<T,U>::Value. */
    template<typename T, typename U>
    inline constexpr bool IsSameV = IsSame<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_SAME_H