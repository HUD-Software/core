#ifndef HD_INC_CORE_TRAITS_IS_LVALUE_REFERENCE_H
#define HD_INC_CORE_TRAITS_IS_LVALUE_REFERENCE_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a lvalue reference type. */
    template<typename type_t>
    struct is_lvalue_reference
        : hud::false_type
    {
    };

    template<typename type_t>
    struct is_lvalue_reference<type_t &>
        : hud::true_type
    {
    };

    /** Equivalent of is_lvalue_reference<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_LVALUE_REFERENCE_H