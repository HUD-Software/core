#ifndef HD_INC_CORE_TRAITS_IS_ARRAY_H
#define HD_INC_CORE_TRAITS_IS_ARRAY_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is an array type ( Bounded or unbounded ). */
    template<typename type_t>
    struct is_array
        : hud::false_type
    {
    };

    template<typename type_t, usize extent>
    struct is_array<type_t[extent]>
        : hud::true_type
    {
    };

    template<typename type_t>
    struct is_array<type_t[]>
        : hud::true_type
    {
    };

    /** Equivalent of hud::is_array<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_array_v = hud::is_array<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_ARRAY_H
