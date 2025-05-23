#ifndef HD_INC_CORE_TRAITS_IS_ARITHMETIC_H
#define HD_INC_CORE_TRAITS_IS_ARITHMETIC_H
#include "disjunction.h"
#include "is_integral.h"
#include "is_floating_point.h"

namespace hud
{

    /**  Checks whether type_t is an arithmetic type (that is, an integral type or a floating-point type) or a cv-qualified version thereof. */
    template<typename type_t>
    struct is_arithmetic
        : hud::disjunction<is_integral<type_t>, is_floating_point<type_t>>
    {
    };

    /** Equivalent of is_arithmetic<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_arithmetic_v = is_arithmetic<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_ARITHMETIC_H
