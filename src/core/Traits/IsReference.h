#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_REFERENCE_H
#define HD_INC_OSLAYER_TRAITS_IS_REFERENCE_H
#include "Or.h"
#include "IsLValueReference.h"
#include "IsRValueReference.h"

namespace hud {

    /**Checks whether T is a reference type (lvalue reference or rvalue reference). */
    template < typename T>
    struct IsReference
        : Or< IsLValueReference<T>, IsRValueReference<T>> {
    };

    /** Equivalent of IsReference<T>::Value */
    template<typename T>
    inline constexpr bool IsReferenceV = IsReference<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_REFERENCE_H
