#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_BOUNDED_ARRAY_H
#define HD_INC_OSLAYER_TRAITS_IS_BOUNDED_ARRAY_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a bounded array type. */
    template<typename T>
    struct IsBoundedArray
        : FalseType {
    };
    template<typename T, usize extent>
    struct IsBoundedArray<T[extent]>
        : TrueType {
    };

    /** Equivalent of IsBoundedArray<T>::Value. */
    template <typename T>
    inline constexpr bool IsBoundedArrayV = IsBoundedArray<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BOUNDED_ARRAY_H