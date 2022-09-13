#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_UNBOUNDED_ARRAY_H
#define HD_INC_OSLAYER_TRAITS_IS_UNBOUNDED_ARRAY_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a unbounded array type. */
    template<typename T>
    struct IsUnboundedArray 
        : FalseType {
    };
    template<typename T>
    struct IsUnboundedArray<T[]>
        : TrueType {
    };

    /** Equivalent of IsUnboundedArray<T>::Value. */
    template <typename T>
    inline constexpr bool IsUnboundedArrayV = IsUnboundedArray<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_UNBOUNDED_ARRAY_H