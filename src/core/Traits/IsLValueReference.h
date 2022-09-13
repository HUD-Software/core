#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_LVALUE_REFERENCE_H
#define HD_INC_OSLAYER_TRAITS_IS_LVALUE_REFERENCE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a lvalue reference type. */
    template <typename T>
    struct IsLValueReference
        : FalseType {
    };
    template <typename T>
    struct IsLValueReference<T&>
        : TrueType {
    };

    /** Equivalent of IsLValueReference<T>::Value. */
    template<typename T>
    inline constexpr bool IsLValueReferenceV = IsLValueReference<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_LVALUE_REFERENCE_H