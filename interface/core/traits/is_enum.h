#ifndef HD_INC_CORE_TRAITS_IS_ENUM_H
#define HD_INC_CORE_TRAITS_IS_ENUM_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is an enumeration type. */
    template<typename type_t>
    struct is_enum
        : public hud::bool_constant<__is_enum(type_t)>
    {
    };

    /** Equivalent of is_enum<type_t>::value. */
    template<typename type_t>
    constexpr bool is_enum_v = is_enum<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_ENUM_H
