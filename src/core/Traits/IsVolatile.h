#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_VOLATILE_H
#define HD_INC_OSLAYER_TRAITS_IS_VOLATILE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a volatile-qualified type. */
    template<typename T>
    struct IsVolatile
        : FalseType {
    };
    template<typename T>
    struct IsVolatile<volatile T>
        : TrueType {
    };

    /** Equivalent of IsVolatile<T>::Value. */
    template<typename T>
    inline constexpr bool IsVolatileV = IsVolatile<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_VOLATILE_H
