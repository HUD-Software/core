#ifndef HD_INC_CORE_TRAITS_IS_REFERENCE_H
#define HD_INC_CORE_TRAITS_IS_REFERENCE_H
#include "disjunction.h"
#include "is_lvalue_reference.h"
#include "is_rvalue_reference.h"

namespace hud
{

    /**Checks whether type_t is a reference type (lvalue reference or rvalue reference). */
    template<typename type_t>
    struct is_reference
        : hud::disjunction<is_lvalue_reference<type_t>, is_rvalue_reference<type_t>>
    {
    };

    /** Equivalent of is_reference<type_t>::value */
    template<typename type_t>
    inline constexpr bool is_reference_v = is_reference<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_REFERENCE_H
