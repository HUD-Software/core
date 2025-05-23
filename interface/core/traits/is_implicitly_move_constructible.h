#ifndef HD_INC_CORE_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H
#include "is_convertible.h"
#include "add_rvalue_reference.h"

namespace hud
{

    /** Checks whether type_t is an implicitly move constructible type with u_type_t. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_implicitly_move_constructible
        : hud::is_convertible<hud::add_rvalue_reference_t<u_type_t>, type_t>
    {
    };

    /** Equivalent of is_implicitly_move_constructible<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_implicitly_move_constructible_v = is_implicitly_move_constructible<type_t, u_type_t>::value;
} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H