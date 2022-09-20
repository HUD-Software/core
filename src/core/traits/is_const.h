#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_CONST_H
#define HD_INC_CORE_TRAITS_IS_CONST_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether type_t is a const-qualified type. */
    template<typename type_t>
    struct is_const 
        : hud::false_type {
    };

    template<typename type_t>
    struct is_const<type_t const>
        : hud::true_type {
    };

    /** Equivalent of is_const<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_const_v = is_const<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BOOL_H
