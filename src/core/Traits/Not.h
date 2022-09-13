#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_NOT_H
#define HD_INC_OSLAYER_TRAITS_NOT_H
#include "IntegralConstant.h"
#include "Conditional.h"

namespace hud {

    /** Performs the logical negation of Value member of T. */
    template<typename T, bool = T::Value>
    struct Not
        : FalseType {
    };
    template<typename T>
    struct Not<T, false>
        : TrueType {
    };

    /** Equivalent of Not<T>::Value. */
    template<typename T>
    inline constexpr bool NotV = Not<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_NOT_H