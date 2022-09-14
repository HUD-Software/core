#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_POINTER_H
#define HD_INC_OSLAYER_traits_IS_POINTER_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether T is a pointer to object or a pointer to function (but not a pointer to member/member function). */
    template<typename T>
    struct IsPointer
        : FalseType {
    };
    template<typename T>
    struct IsPointer<T*>
        : TrueType {
    };

    /** Equivalent of IsPointer<T>::Value. */
    template<typename T>
    inline constexpr bool IsPointerV = IsPointer<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_POINTER_H