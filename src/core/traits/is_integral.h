#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_INTEGRAL_H
#define HD_INC_CORE_TRAITS_IS_INTEGRAL_H
#include "is_any_type_in.h"
#include "remove_cv.h"
#include "integral_constant.h"


namespace hud {

    /**  Checks whether type_t is an integral type. */
    template<typename type_t>
    struct is_integral 
        : is_any_type_in<remove_cv_t<type_t>, bool, char, signed char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long, wchar_t, char8_t, char16_t, char32_t> {
    };

    /** Equivalent of is_integral<type_t>::value */
    template<typename type_t>
    inline constexpr bool is_integral_v = is_integral<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_INTEGRAL_H