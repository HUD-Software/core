#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_CONST_H
#define HD_INC_OSLAYER_TRAITS_IS_CONST_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a const-qualified type. */
    template<typename T>
    struct IsConst 
        : FalseType {
    };

    template<typename T>
    struct IsConst<T const>
        : TrueType {
    };

    /** Equivalent of IsConst<T>::Value. */
    template<typename T>
    inline constexpr bool IsConstV = IsConst<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BOOL_H
