#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_NULLPTR_H
#define HD_INC_CORE_TRAITS_IS_NULLPTR_H
#include "integral_constant.h"
#include "is_same.h"
#include "remove_cv.h"

namespace hud {

    /** Checks whether type_t is hud::ptr::null or std::nullptr_t type. */
    template<typename type_t>
    struct is_nullptr
        : hud::is_same<remove_cv_t<type_t>, decltype(nullptr)> {
    };

    /** Equivalent of is_nullptr<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_nullptr_v = is_nullptr<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NULLPTR_H