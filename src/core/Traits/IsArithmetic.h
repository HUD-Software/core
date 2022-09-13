#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_ARITHMETIC_H
#define HD_INC_OSLAYER_TRAITS_IS_ARITHMETIC_H
#include "Or.h"
#include "IsIntegral.h"
#include "IsFloatingPoint.h"

namespace hud {

    /**  Checks whether T is an arithmetic type (that is, an integral type or a floating-point type) or a cv-qualified version thereof. */
    template<typename T>
    struct IsArithmetic
        : Or< IsIntegral<T>, IsFloatingPoint<T> > {
    };

    /** Equivalent of IsArithmetic<T>::Value. */
    template<typename T>
    inline constexpr bool IsArithmeticV = IsArithmetic<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_ARITHMETIC_H

