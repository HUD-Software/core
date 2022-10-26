#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_BITWISE_COMPARABLE_H
#define HD_INC_CORE_TRAITS_IS_BITWISE_COMPARABLE_H
#include "conjunction.h"
#include "is_bitwise_convertible.h"
#include "disjunction.h"
#include "is_same.h"
#include "has_unique_object_representations.h"
#include "is_standard_layout.h"

namespace hud
{

    /**
     * Checks whether type_t is a bitwise comparable with u_type_t.
     * There is no good way to detect this, specify this trait for your type to allow bitwise comparison.
     * This traits is specialize with Value equal to true if:
     * - type_t and u_type_t are pointers or pointers to member function
     * - type_t and u_type_t are integrals with same size
     * - type_t and u_type_t are enums with underlying type with same size
     */
    template <typename type_t, typename u_type_t = type_t>
    struct is_bitwise_comparable
        : hud::disjunction<
              hud::is_bitwise_convertible<type_t, u_type_t>,
              hud::conjunction< // or if same POD type. POD
                  hud::is_same<type_t, u_type_t>,
                  hud::conjunction<has_unique_object_representations<type_t>>,
                  hud::conjunction<is_standard_layout<type_t>>>>
    {
    };

    /** Equivalent of is_bitwise_comparable<u_type_t, V=u_type_t>::value. */
    template <typename u_type_t, typename V = u_type_t>
    inline constexpr bool is_bitwise_comparable_v = is_bitwise_comparable<u_type_t, V>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BITWISE_COMPARABLE_H
