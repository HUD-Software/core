#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a constructible type using the set of argument types specified by args_t, and such construction is known not to throw any exception. */
    template<typename type_t, typename... args_t>
    struct is_nothrow_constructible
        : hud::bool_constant<__is_nothrow_constructible(type_t, args_t...)>
    {
    };

    /** Equivalent of is_nothrow_constructible<type_t, args_t...>::value. */
    template<typename type_t, typename... args_t>
    constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<type_t, args_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_H