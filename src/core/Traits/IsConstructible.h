#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_CONSTRUCTIBLE_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is a constructible type with the set of argument types specified by Arg (T obj(Declval<Args>()...); is well - formed).
    * A constructible type is a type that can be constructed using a particular set of arguments.
    */
    template<typename T, typename... TArgs>
    struct IsConstructible
        : BoolConstant<__is_constructible(T, TArgs...)> {
    };

    /** Equivalent of IsConstructible<T, TArgs...>::Value. */
    template<typename T, typename... TArgs>
    inline constexpr bool IsConstructibleV = IsConstructible<T, TArgs...>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_CONSTRUCTIBLE_H