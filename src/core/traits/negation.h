#ifndef HD_INC_CORE_TRAITS_NOT_H
#define HD_INC_CORE_TRAITS_NOT_H
#include "integral_constant.h"
#include "conditional.h"

namespace hud
{

    /** Performs the logical negation of value member of type_t. */
    template<typename type_t, bool = type_t::value>
    struct negation
        : hud::false_type
    {
    };

    template<typename type_t>
    struct negation<type_t, false>
        : hud::true_type
    {
    };

    /** Equivalent of hud::negation<type_t>::value. */
    template<typename type_t>
    inline constexpr bool negation_v = hud::negation<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_NOT_H