#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_REFERENCE_H
#define HD_INC_OSLAYER_traits_IS_REFERENCE_H
#include "or.h"
#include "is_lvalue_reference.h"
#include "is_rvalue_reference.h"

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

#endif // HD_INC_OSLAYER_traits_IS_REFERENCE_H
