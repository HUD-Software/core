#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_OR_H
#define HD_INC_OSLAYER_TRAITS_OR_H
#include "Conditional.h"
#include "IntegralConstant.h"

namespace hud {
    
    /** Performs the logical disjunction of the type traits B..., effectively performing a logical OR on the sequence of traits. */
    template<typename...>
    struct Or;

    template<>
    struct Or<> 
        : public FalseType {
        // Empty is false
    };

    template<typename B>
    struct Or<B> 
        : B {
        // Only 1 boolean, evaluate without conditional
    };

    template<typename B, typename... Rest>
    struct Or<B, Rest...> 
        : ConditionalT<B::Value, B, Or<Rest...>> {
        // If B is false evaluate next, else B is true
    };

    /** Equivalent of Or<T...>::Value */
    template<typename... T>
    inline constexpr bool OrV = Or<T...>::Value;

    /** Equivalent of Or<T...>::Type */
    template<typename... T>
    using OrT = typename Or<T...>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_OR_H
