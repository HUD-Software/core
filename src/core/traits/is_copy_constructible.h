#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_COPY_CONSTRUCTIBLE_H
#include "add_const.h"
#include "is_constructible.h"
#include "add_lvalue_reference.h"

namespace hud
{

    /**
     * Checks whether u_type_t is a copy constructible type to type_t.
     * A copy constructible type is a type that can be constructed from a value or reference of the same type or another type.
     * A copy constructible class is a class that has a copy constructor (either its implicit constructor or a custom defined one)
     */
    template <typename type_t, typename u_type_t = type_t>
    struct is_copy_constructible
        : is_constructible<type_t, hud::add_lvalue_reference_t<hud::add_const_t<u_type_t>>>
    {
    };

    /** Equivalent of hud::is_copy_constructible<type_t, u_type_t>::value. */
    template <typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_copy_constructible_v = hud::is_copy_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_COPY_CONSTRUCTIBLE_H