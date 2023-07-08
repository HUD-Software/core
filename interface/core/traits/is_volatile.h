#ifndef HD_INC_CORE_TRAITS_IS_VOLATILE_H
#define HD_INC_CORE_TRAITS_IS_VOLATILE_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a volatile-qualified type. */
    template<typename type_t>
    struct is_volatile
        : hud::false_type
    {
    };

    template<typename type_t>
    struct is_volatile<volatile type_t>
        : hud::true_type
    {
    };

    /** Equivalent of is_volatile<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_volatile_v = is_volatile<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_VOLATILE_H
