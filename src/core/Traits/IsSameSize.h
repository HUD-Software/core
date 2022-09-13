#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_SAME_SIZE_H
#define HD_INC_OSLAYER_TRAITS_IS_SAME_SIZE_H
#include "IntegralConstant.h"
#include "RemoveAllExtents.h"

namespace hud {

    /** Checks whether RemoveAllExtentT<T> is the same size has RemoveAllExtentT<U>. */
    template<typename T, typename U>
    struct IsSameSize
        : BoolConstant<sizeof(RemoveAllExtentsT<T>) == sizeof(RemoveAllExtentsT<U>)> {
    };

    /** Equivalent of IsSameSize<T,U>::Value. */
    template<typename T, typename U >
    inline constexpr bool IsSameSizeV = IsSameSize<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_SAME_SIZE_H
