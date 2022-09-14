#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_VOID_H
#define HD_INC_OSLAYER_traits_IS_VOID_H
#include "integral_constant.h"
#include "is_same.h"
#include "remove_cv.h"

namespace hud {

    /** Checks whether T is a void type. */
    template<typename T>
    struct IsVoid
        : IsSame<void, RemoveCVT<T>> {
    };

    /** Equivalent of IsVoid<T>::Value. */
    template<typename T>
    inline constexpr bool IsVoidV = IsVoid<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_VOID_H