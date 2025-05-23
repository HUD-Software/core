#ifndef HD_INC_CORE_TRAITS_IS_RVALUE_REFERENCE_H
#define HD_INC_CORE_TRAITS_IS_RVALUE_REFERENCE_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a rvalue reference type. */
    template<typename type_t>
    struct is_rvalue_reference
        : hud::false_type
    {
    };

    template<typename type_t>
    struct is_rvalue_reference<type_t &&>
        : hud::true_type
    {
    };

    /** Equivalent of is_rvalue_reference<type_t>::value. */
    template<typename type_t>
    constexpr bool is_rvalue_reference_v = is_rvalue_reference<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_RVALUE_REFERENCE_H