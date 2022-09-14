#pragma once
#ifndef HD_INC_CORE_TRAITS_OR_H
#define HD_INC_CORE_TRAITS_OR_H
#include "conditional.h"
#include "integral_constant.h"

namespace hud {
    
    /** Performs the logical disjunction of the type traits type_t..., effectively performing a logical OR on the sequence of traits. */
    template<typename...>
    struct disjunction;

    template<>
    struct disjunction<> 
        : public false_type {
        // Empty is false
    };

    template<typename type_t>
    struct disjunction<type_t> 
        : type_t {
        // Only 1 boolean, evaluate without conditional
    };

    template<typename type_t, typename... rest_t>
    struct disjunction<type_t, rest_t...> 
        : conditional_t<type_t::value, type_t, disjunction<rest_t...>> {
        // If type_t is false evaluate next, else type_t is true
    };

    /** Equivalent of disjunction<type_t...>::value */
    template<typename... type_t>
    inline constexpr bool or_v = disjunction<type_t...>::value;

    /** Equivalent of disjunction<type_t...>::type */
    template<typename... type_t>
    using or_t = typename disjunction<type_t...>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_OR_H
