#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether type_t is a trivially constructible type with the set of argument types specified by Arg.
     * Trivially constructible type_t is a type which can be trivially default-, copy- or move- constructed, depending on the particular set of argument types used as args_t
     */
    template <typename type_t, typename... args_t>
    struct is_trivially_constructible
        : hud::bool_constant<__is_trivially_constructible(type_t, args_t...)>
    {
    };

    /** Equivalent of is_trivially_constructible<type_t, args_t...>::value. */
    template <typename type_t, typename... args_t>
    inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<type_t, args_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H