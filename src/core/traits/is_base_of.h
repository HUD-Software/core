#ifndef HD_INC_CORE_TRAITS_IS_BASE_OF_H
#define HD_INC_CORE_TRAITS_IS_BASE_OF_H
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether base_t is a base class of (or the same class as) derived_t, without regard to their const and/or volatile qualification.
     * Only classes that are not unions are considered.
     */
    template<typename base_t, typename derived_t>
    struct is_base_of
        : hud::bool_constant<__is_base_of(base_t, derived_t)>
    {
    };

    /** Equivalent of is_base_of<base_t, derived_t>::value. */
    template<typename base_t, typename derived_t>
    inline constexpr bool is_base_of_v = is_base_of<base_t, derived_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BASE_OF_H