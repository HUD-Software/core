#pragma once
#ifndef HD_INC_CORE_TRAITS_CONDITIONAL_H
#define HD_INC_CORE_TRAITS_CONDITIONAL_H

namespace hud {

    /** Provides member typedef type, which is defined as if_true_t if condition is true at compile time, or as if_false_t if condition is false. */
    template<bool condition, typename if_true_t, typename if_false_t>
    struct conditional {
        using type = if_true_t;
    };
    template<typename if_true_t, typename if_false_t>
    struct conditional<false, if_true_t, if_false_t> {
        using type = if_false_t;
    };

    /** Equivalent of typename conditional<condition, if_true_t, if_false_t>::type. */
    template<bool condition, typename if_true_t, typename if_false_t>
    using conditional_t = typename conditional<condition, if_true_t, if_false_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_CONDITIONAL_H
