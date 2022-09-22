#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether type_t is a default constructible type, and such construction is known not to throw any exception. */
    template <typename type_t>
    struct is_nothrow_default_constructible
        : hud::bool_constant<__is_nothrow_constructible(type_t)> {
    };

    /** Equivalent of is_nothrow_default_constructible<type_t>::value. */
    template <typename type_t>
    inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<type_t>::value;





} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_H