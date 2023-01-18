#ifndef HD_INC_CORE_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#include "is_implicitly_constructible.h"

namespace hud
{

    /** Checks whether type_t is an implicitly default constructible type. */
    template<typename type_t>
    struct is_implicitly_default_constructible
        : is_implicitly_constructible<type_t>
    {
    };

    /** Equivalent of is_implicitly_default_constructible<type_t>::value */
    template<typename type_t>
    inline constexpr bool is_implicitly_default_constructible_v = is_implicitly_default_constructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H