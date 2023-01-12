#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_SWAPPABLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_SWAPPABLE_H
#include "conjunction.h"
#include "is_trivially_destructible.h"
#include "is_trivially_move_constructible.h"
#include "is_trivially_move_assignable.h"
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether type_t is trivially swappable with u_type_t.
     * A trivially swappable type are trivially destructible, trivially move constructible and trivially move assignable.
     */
    template <typename type_t, typename u_type_t = type_t>
    struct is_trivially_swappable
        : hud::conjunction<is_trivially_move_constructible<type_t, u_type_t>, is_trivially_move_assignable<type_t, u_type_t>, is_trivially_destructible<type_t>, is_trivially_destructible<u_type_t>>
    {
    };

    /** Equivalent of is_trivially_swappable<type_t, u_type_t>::value */
    template <typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_trivially_swappable_v = is_trivially_swappable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_SWAPPABLE_H