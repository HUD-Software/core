#ifndef HD_INC_CORE_TRAITS_IS_SIGNED_H
#define HD_INC_CORE_TRAITS_IS_SIGNED_H
#include "integral_constant.h"
#include "is_integral.h"
#include "is_floating_point.h"
#include "remove_cv.h"

namespace hud
{

    /**
     * Checks whether type_t is a signed arithmetic type.
     * A type, type_t, is considered signed by this class if :
     * - It is an arithmetic type (i.e., a fundamental integral or floating point type).
     * - type_t(-1) < type_t(0) is true.
     */
    template<typename type_t, bool = is_integral_v<type_t>>
    struct is_signed
        : hud::bool_constant<hud::remove_cv_t<type_t>(-1) < hud::remove_cv_t<type_t>(0)>
    {
    };

    template<typename type_t>
    struct is_signed<type_t, false>
        : hud::is_floating_point<type_t>
    {
    };

    /** Equivalent of is_signed<type_t>::value. */
    template<typename type_t>
    constexpr bool is_signed_v = hud::is_signed<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_SIGNED_H