#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_RVALUE_REFERENCE_H
#define HD_INC_OSLAYER_TRAITS_IS_RVALUE_REFERENCE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a rvalue reference type. */
    template <typename T>
    struct IsRValueReference
        : FalseType {
    };
    template <typename T>
    struct IsRValueReference<T&&>
        : TrueType {
    };

    /** Equivalent of IsRValueReference<T>::Value. */
    template<typename T>
    inline constexpr bool IsRValueReferenceV = IsRValueReference<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_RVALUE_REFERENCE_H