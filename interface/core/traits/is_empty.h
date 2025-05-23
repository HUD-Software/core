#ifndef HD_INC_CORE_TRAITS_IS_EMPTY_H
#define HD_INC_CORE_TRAITS_IS_EMPTY_H
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether `type_t` is an empty type.
     * An empty type is a type that:
     *  - is a non-union class type with no non-static data members other than bit-fields of size 0
     *  - has no virtual functions
     *  - has no virtual base classes
     *  - has no non-empty base classes
     */
    template<typename type_t>
    struct is_empty
        : hud::bool_constant<__is_empty(type_t)>
    {
    };

    /** Equivalent of is_empty<type_t>::value. */
    template<typename type_t>
    constexpr bool is_empty_v = is_empty<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_EMPTY_H