#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NULLPTR_H
#define HD_INC_OSLAYER_TRAITS_IS_NULLPTR_H
#include "IntegralConstant.h"
#include "IsSame.h"
#include "RemoveCV.h"

namespace hud {

    /** Checks whether T is hud::ptr::null or std::nullptr_t type. */
    template<typename T>
    struct IsNullptr
        : IsSame<RemoveCVT<T>, decltype(nullptr)> {
    };

    /** Equivalent of IsNullptrV<T>::Value. */
    template<typename T>
    inline constexpr bool IsNullptrV = IsNullptr<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NULLPTR_H