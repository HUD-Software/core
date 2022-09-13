#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_SWAPPABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_SWAPPABLE_H
#include "IntegralConstant.h"
#include "VoidT.h"
#include "../Templates/Declval.h"
#include "../Templates/Swap.h"

namespace hud {

    /** Check whether T is swappable with U. */
    template<typename T, typename U = T, typename = void>
    struct IsSwappable
        : FalseType {
    };
    template<typename T, typename U>
    struct IsSwappable<T, U, VoidT<decltype(swap(declval<AddLValueReferenceT<T>>(), declval<AddLValueReferenceT<U>>()))>>
        : TrueType {
    };

    /** Equivalent of IsSwappable<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsSwappableV = IsSwappable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_SWAPPABLE_H