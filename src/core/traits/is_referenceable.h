#ifndef HD_INC_CORE_TRAITS_IS_REFERENCEABLE_H
#define HD_INC_CORE_TRAITS_IS_REFERENCEABLE_H
#include "integral_constant.h"
#include "void_t.h"

namespace hud
{

    /** Check if a type is referenceable (type_t& is well-formed). */
    template<typename type_t, typename = void>
    struct is_referenceable
        : hud::false_type
    {
    };

    template<typename type_t>
    struct is_referenceable<type_t, void_t<type_t &>>
        : hud::true_type
    {
    };

    /** Equivalent of is_referenceable<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_referenceable_v = is_referenceable<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_REFERENCEABLE_H