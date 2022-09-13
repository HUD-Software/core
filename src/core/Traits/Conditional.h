#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_CONDITIONAL_H
#define HD_INC_OSLAYER_TRAITS_CONDITIONAL_H

namespace hud {

    /** Provides member typedef Type, which is defined as IfTrue if condition is true at compile time, or as IfFalse if condition is false. */
    template<bool condition, typename IfTrue, typename IfFalse>
    struct Conditional {
        using Type = IfTrue;
    };
    template<typename IfTrue, typename IfFalse>
    struct Conditional<false, IfTrue, IfFalse> {
        using Type = IfFalse;
    };

    /** Equivalent of typename Conditional<condition, IfTrue, IfFalse>::Type. */
    template<bool condition, typename IfTrue, typename IfFalse>
    using ConditionalT = typename Conditional<condition, IfTrue, IfFalse>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_CONDITIONAL_H
