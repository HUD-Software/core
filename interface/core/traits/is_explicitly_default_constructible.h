#ifndef HD_INC_CORE_TRAITS_IS_EXPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_EXPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#include "is_implicitly_default_constructible.h"

namespace hud
{

    /** Checks whether type_t is an explicitly default constructible type. */
    template<typename type_t>
    struct is_explicitly_default_constructible
        : hud::conjunction<
              hud::is_default_constructible<type_t>,
              hud::negation<hud::is_implicitly_default_constructible<type_t>>>
    {
    };

    /** Equivalent of hud::is_explicitly_default_constructible<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_explicitly_default_constructible_v = hud::is_explicitly_default_constructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H