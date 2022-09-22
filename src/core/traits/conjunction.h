#pragma once
#ifndef HD_INC_CORE_TRAITS_ADD_H
#define HD_INC_CORE_TRAITS_ADD_H
#include "conditional.h"
#include "integral_constant.h"

namespace hud {

    /** Performs the logical conjunction of the type traits type_t..., effectively performing a logical AND on the sequence of traits. */
    template< typename... >
    struct conjunction;

    template<>
    struct hud::conjunction<>
        : hud::true_type {
        // Empty is true
    };

    template<typename type_t>
    struct hud::conjunction<type_t>
        : type_t {
        // Only 1 boolean, evaluate without conditional
    };

    template<typename type_t, typename... rest_t>
    struct hud::conjunction<type_t, rest_t...>
        : hud::conditional_t<type_t::value, hud::conjunction<rest_t...>, type_t> {
        // If type_t is true evaluate next, else type_t is false
    };


    /** Equivalent of hud::conjunction<type_t...>::value. */
    template<typename... type_t>
    inline constexpr bool conjunction_v = hud::conjunction<type_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_ADD_H
