#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_INTEGRAL_H
#define HD_INC_OSLAYER_TRAITS_IS_INTEGRAL_H
#include "IsAnyTypeIn.h"
#include "RemoveCV.h"
#include "IntegralConstant.h"


namespace hud {

    /**  Checks whether T is an integral type. */
    template<typename T>
    struct IsIntegral 
        : IsAnyTypeIn<RemoveCVT<T>, bool, char, signed char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long, wchar_t, char8_t, char16_t, char32_t> {
    };

    /** Equivalent of IsIntegral<T>::Value */
    template<typename T>
    inline constexpr bool IsIntegralV = IsIntegral<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_INTEGRAL_H