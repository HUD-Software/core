#ifndef HD_INC_CORE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H
#include "conjunction.h"
#include "is_constructible.h"
#include "negation.h"
#include "is_implicitly_constructible.h"

namespace hud
{
    /** Checks whether type_t is an explicitly constructible type with args_t. */
    template<typename type_t, typename... args_t>
    struct is_explicitly_constructible
        : hud::conjunction<hud::is_constructible<type_t, args_t...>, hud::negation<hud::is_implicitly_constructible<type_t, args_t...>>>
    {
    };

    /** Equivalent of is_explicitly_constructible<type_t, args_t...>::value. */
    template<typename type_t, typename... args_t>
    constexpr bool is_explicitly_constructible_v = is_explicitly_constructible<type_t, args_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H