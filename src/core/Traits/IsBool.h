#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_BOOL_H
#define HD_INC_OSLAYER_TRAITS_IS_BOOL_H
#include "IsSame.h"
#include "RemoveCV.h"

namespace hud {

    /** Checks whether T is a bool type. */
    template<typename T>
    struct IsBool
        : IsSame<RemoveCVT<T>, bool> {
    };

    /** Equivalent of IsBool<T>::Value. */
    template<typename T>
    inline constexpr bool IsBoolV = IsBool<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BOOL_H