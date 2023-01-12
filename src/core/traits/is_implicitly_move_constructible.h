#ifndef HD_INC_CORE_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H
#include "add_rvalue_reference.h"
#include "is_implicitly_constructible.h"

namespace hud
{

    /** Checks whether type_t is an implicitly move constructible type with u_type_t. */
    template <typename type_t, typename u_type_t = type_t>
    struct is_implicitly_move_constructible
        : is_implicitly_constructible<type_t, add_rvalue_reference_t<u_type_t>>
    {
    };

    /** Equivalent of is_implicitly_move_constructible<type_t, u_type_t>::value. */
    template <typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_implicitly_move_constructible_v = is_implicitly_move_constructible<type_t, u_type_t>::value;
}

#endif // HD_INC_CORE_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H