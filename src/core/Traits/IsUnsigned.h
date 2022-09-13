#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_UNSIGNED_H
#define HD_INC_OSLAYER_TRAITS_IS_UNSIGNED_H
#include "IntegralConstant.h"
#include "And.h"
#include "IsIntegral.h"
#include "RemoveCV.h"
namespace hud {

    /**
    * Checks whether T is a unsigned integral type.
    * A type, T, is considered unsigned by this class if :
    * - It is an unsigned integral or boolean.
    * - T(0) < T(-1) is true.
    */
    template<typename T, bool = IsIntegralV<T>>
    struct IsUnsigned
        : FalseType {
    };
    template<typename T>
    struct IsUnsigned<T, true>
        : BoolConstant < RemoveCVT<T>(0) < RemoveCVT<T>(-1) > {
    };

    /** Equivalent of IsUnsigned<T>::Value. */
    template<typename T>
    inline constexpr bool IsUnsignedV = IsUnsigned<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_UNSIGNED_H