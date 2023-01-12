#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_FLOATING_POINT_H
#define HD_INC_CORE_TRAITS_IS_FLOATING_POINT_H
#include "is_one_of_types.h"
#include "remove_cv.h"
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a floating-point type. */
    template <typename type_t>
    struct is_floating_point
        : public is_one_of_types<remove_cv_t<type_t>, float, double, long double>
    {
    };

    /**Equivalent of is_floating_point<type_t>::Valu. */
    template <typename type_t>
    inline constexpr bool is_floating_point_v = is_floating_point<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_FLOATING_POINT_H