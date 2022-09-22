#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_VOID_H
#define HD_INC_CORE_TRAITS_IS_VOID_H
#include "integral_constant.h"
#include "is_same.h"
#include "remove_cv.h"

namespace hud {

    /** Checks whether type_t is a void type. */
    template<typename type_t>
    struct is_void
        : hud::is_same<void, remove_cv_t<type_t>> {
    };

    /** Equivalent of is_void<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_void_v = is_void<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_VOID_H