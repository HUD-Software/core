#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_ADD_H
#define HD_INC_OSLAYER_TRAITS_ADD_H
#include "Conditional.h"
#include "IntegralConstant.h"

namespace hud {

    /** Performs the logical conjunction of the type traits T..., effectively performing a logical AND on the sequence of traits. */
    template<typename...>
    struct And;

    template<>
    struct And<>
        : TrueType {
        // Empty is true
    };

    template<typename B>
    struct And<B>
        : B {
        // Only 1 boolean, evaluate without conditional
    };

    template<typename B, typename... Rest>
    struct And<B, Rest...>
        : ConditionalT<B::Value, And<Rest...>, B> {
        // If B is true evaluate next, else B is false
    };


    /** Equivalent of And<T...>::Value. */
    template<typename... T>
    inline constexpr bool AndV = And<T...>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_ADD_H
