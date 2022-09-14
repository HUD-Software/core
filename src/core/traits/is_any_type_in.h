#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_ANY_TYPE_IN_H
#define HD_INC_CORE_TRAITS_IS_ANY_TYPE_IN_H
#include "disjunction.h"
#include "is_same.h"

namespace hud {

    /** Check if the type to_find_t is in list_t of types. */
    template<typename to_find_t, typename... list_t>
    struct is_any_type_in
        : disjunction<is_same<to_find_t, list_t>...> {
    };

    /** Equivalent of is_any_type_in<to_find_t, list_t...>::value. */
    template<typename to_find_t, typename... list_t>
    inline constexpr bool is_any_type_in_v = is_any_type_in<to_find_t, list_t...>::value;
}

#endif // HD_INC_CORE_TRAITS_IS_ANY_TYPE_IN_H
