#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_ARRAY_H
#define HD_INC_OSLAYER_TRAITS_IS_ARRAY_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is an array type ( Bounded or unbounded ). */
    template<typename T>
    struct IsArray
        : FalseType {
    };
    template <typename T, usize extent>
    struct IsArray<T[extent]>
        : TrueType {
    };
    template <typename T>
    struct IsArray<T[]>
        : TrueType {
    };

    /** Equivalent of IsArray<T>::Value. */
    template <typename T>
    inline constexpr bool IsArrayV = IsArray<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_ARRAY_H
