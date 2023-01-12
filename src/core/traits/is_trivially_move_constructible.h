#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE_H
#include "is_trivially_constructible.h"
#include "add_rvalue_reference.h"

namespace hud
{

    /**
     * Checks whether u_type_t is a trivially move constructible type to type_t.
     * A trivially move constructible type is a type which can be trivially constructed from an rvalue reference of its type or another type
     * This includes scalar types, trivially move constructible classes and arrays of such types.
     * A trivially move constructible class is a class (defined with class, struct or union) that:
     *   - uses the implicitly defined move constructor.
     *   - has no virtual members.
     *   - its base class and non-static data members (if any) are themselves also trivially move constructible types.
     */
    template <typename type_t, typename u_type_t = type_t>
    struct is_trivially_move_constructible
        : is_trivially_constructible<type_t, add_rvalue_reference_t<u_type_t>>
    {
    };

    /** Equivalent of is_trivially_move_constructible<type_t,u_type_t>::value. */
    template <typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE_H