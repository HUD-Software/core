#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_ENUM_H
#define HD_INC_OSLAYER_TRAITS_IS_ENUM_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is an enumeration type. */
    template<typename T>
    struct IsEnum
        : public BoolConstant<__is_enum(T)> {
    };

    /** Equivalent of IsEnum<T>::Value. */
    template<typename T>
    inline constexpr bool IsEnumV = IsEnum<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_ENUM_H
