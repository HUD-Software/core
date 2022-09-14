#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_FLOATING_POINT_H
#define HD_INC_OSLAYER_traits_IS_FLOATING_POINT_H
#include "is_any_type_in.h"
#include "remove_cv.h"
#include "integral_constant.h"

namespace hud {


    /** Checks whether T is a floating-point type. */
    template<typename T>
    struct IsFloatingPoint
        : public IsAnyTypeIn<RemoveCVT<T>, float, double, long double> {
    };

    /**Equivalent of IsFloatingPoint<T>::Valu. */
    template<typename T>
    inline constexpr bool IsFloatingPointV = IsFloatingPoint<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_FLOATING_POINT_H