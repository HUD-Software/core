#ifndef HD_INC_CORE_TRAITS_IS_BOOL_H
#define HD_INC_CORE_TRAITS_IS_BOOL_H
#include "is_same.h"
#include "remove_cv.h"

namespace hud
{

    /** Checks whether type_t is a bool type. */
    template<typename type_t>
    struct is_bool
        : hud::is_same<remove_cv_t<type_t>, bool>
    {
    };

    /** Equivalent of is_bool<type_t>::value. */
    template<typename type_t>
    constexpr bool is_bool_v = is_bool<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BOOL_H