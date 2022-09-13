#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_SIGNED_H
#define HD_INC_OSLAYER_TRAITS_IS_SIGNED_H
#include "IntegralConstant.h"
#include "IsIntegral.h"
#include "IsFloatingPoint.h"
#include "RemoveCV.h"

namespace hud {

    /**
    * Checks whether T is a signed arithmetic type.
    * A type, T, is considered signed by this class if :
    * - It is an arithmetic type (i.e., a fundamental integral or floating point type).
    * - T(-1) < T(0) is true.
    */
    template<typename T, bool = IsIntegralV<T>>
    struct IsSigned
        : BoolConstant < RemoveCVT<T>(-1) < RemoveCVT<T>(0) > {
    };
    template<typename T>
    struct IsSigned<T, false>
        : IsFloatingPoint<T> {
    };

    /** Equivalent of IsSigned<T>::Value. */
    template<typename T>
    inline constexpr bool IsSignedV = IsSigned<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_SIGNED_H