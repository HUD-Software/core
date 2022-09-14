#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_H
#include "is_nothrow_assignable.h"
#include "add_lvalue_reference.h"
#include "add_rvalue_reference.h"

namespace hud {

    /** Checks whether type_t is move assignable type from u_type_t, and such construction is known not to throw any exception. */
    template <typename type_t, typename u_type_t = type_t>
    struct is_nothrow_move_assignable
        : is_nothrow_assignable<add_lvalue_reference_t<type_t>, add_rvalue_reference_t<u_type_t>> {
    };

    /** Equivalent of is_nothrow_move_assignable<type_t,u_type_t>::value. */
    template <typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_H