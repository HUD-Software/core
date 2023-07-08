#ifndef HD_INC_CORE_TRAITS_IS_CLASS_H
#define HD_INC_CORE_TRAITS_IS_CLASS_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a class type. */
    template<typename type_t>
    struct is_class
        : hud::bool_constant<__is_class(type_t)>
    {
    };

    /** Equivalent of is_class<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_class_v = is_class<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_CLASS_H
