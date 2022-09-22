#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_CONSTRUCTIBLE_H
#include "integral_constant.h"

namespace hud {

    /**
    * Checks whether type_t is a constructible type with the set of argument types specified by Arg (type_t obj(Declval<Args>()...); is well - formed).
    * A constructible type is a type that can be constructed using a particular set of arguments.
    */
    template<typename type_t, typename... args_t>
    struct is_constructible
        : hud::bool_constant<__is_constructible(type_t, args_t...)> {
    };

    /** Equivalent of is_constructible<type_t, args_t...>::value. */
    template<typename type_t, typename... args_t>
    inline constexpr bool is_constructible_v = is_constructible<type_t, args_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_CONSTRUCTIBLE_H