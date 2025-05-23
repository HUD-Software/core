#ifndef HD_INC_CORE_TRAITS_IS_SWAPPABLE_H
#define HD_INC_CORE_TRAITS_IS_SWAPPABLE_H
#include "integral_constant.h"
#include "void_t.h"
#include "../templates/declval.h"
#include "../templates/swap.h"

namespace hud
{

    /** Check whether type_t is swappable with u_type_t. */
    template<typename type_t, typename u_type_t = type_t, typename = void>
    struct is_swappable
        : hud::false_type
    {
    };

    template<typename type_t, typename u_type_t>
    struct is_swappable<type_t, u_type_t, void_t<decltype(swap(declval<hud::add_lvalue_reference_t<type_t>>(), declval<hud::add_lvalue_reference_t<u_type_t>>()))>>
        : hud::true_type
    {
    };

    /** Equivalent of is_swappable<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_swappable_v = is_swappable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_SWAPPABLE_H