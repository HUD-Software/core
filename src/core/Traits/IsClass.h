#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_CLASS_H
#define HD_INC_OSLAYER_TRAITS_IS_CLASS_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a class type. */
    template<typename T>
    struct IsClass
        : BoolConstant<__is_class(T)> {
    };

    /** Equivalent of IsClass<T>::Value. */
    template<typename T>
    inline constexpr bool IsClassV = IsClass<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_CLASS_H
