#pragma once
#ifndef HD_INC_OSLAYER_traits_NOT_H
#define HD_INC_OSLAYER_traits_NOT_H
#include "integral_constant.h"
#include "conditional.h"

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

#endif // HD_INC_OSLAYER_traits_NOT_H