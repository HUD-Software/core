#ifndef HD_INC_CORE_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#include "is_implicitly_copy_constructible.h"

namespace hud
{

    /** Checks whether type_t is an explicitly copy constructible type with u_type_t. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_explicitly_copy_constructible
        : hud::conjunction<
              hud::is_copy_constructible<type_t, u_type_t>,
              hud::negation<hud::is_implicitly_copy_constructible<type_t, u_type_t>>>
    {
    };

    /** Equivalent of hud::is_explicitly_copy_constructible<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_explicitly_copy_constructible_v = hud::is_explicitly_copy_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H