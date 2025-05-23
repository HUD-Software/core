#ifndef HD_INC_CORE_TRAITS_IS_UNSIGNED_H
#define HD_INC_CORE_TRAITS_IS_UNSIGNED_H
#include "integral_constant.h"
#include "conjunction.h"
#include "is_integral.h"
#include "remove_cv.h"

namespace hud
{

    /**
     * Checks whether type_t is a unsigned integral type.
     * A type, type_t, is considered unsigned by this class if :
     * - It is an unsigned integral or boolean.
     * - type_t(0) < type_t(-1) is true.
     */
    template<typename type_t, bool = is_integral_v<type_t>>
    struct is_unsigned
        : hud::false_type
    {
    };

    template<typename type_t>
    struct is_unsigned<type_t, true>
        : hud::bool_constant<remove_cv_t<type_t>(0) < remove_cv_t<type_t>(-1)>
    {
    };

    /** Equivalent of is_unsigned<type_t>::value. */
    template<typename type_t>
    constexpr bool is_unsigned_v = is_unsigned<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_UNSIGNED_H