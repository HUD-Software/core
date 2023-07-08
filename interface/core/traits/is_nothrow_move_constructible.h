#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_H
#include "is_nothrow_constructible.h"
#include "add_rvalue_reference.h"

namespace hud
{

    /** Checks whether type_t is move constructible type from u_type_t, and such construction is known not to throw any exception. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_nothrow_move_constructible
        : is_nothrow_constructible<type_t, add_rvalue_reference_t<u_type_t>>
    {
    };

    /** Checks whether u_type_t is move constructible type from V, and such construction is known not to throw any exception. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_H
