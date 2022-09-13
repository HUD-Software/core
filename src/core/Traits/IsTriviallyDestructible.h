#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is a trivially destructible type.
    * Trivially destructible types include scalar types, trivially copy constructible classes and arrays of such types.
    * A trivially destructible class is a class (defined with class, struct or union) that:
    * - uses the implicitly defined destructor.
    * - the destructor is not virtual.
    * - its base class and non-static data members (if any) are themselves also trivially destructible types.
    */
    template<typename T>
    struct IsTriviallyDestructible
        : BoolConstant<__is_trivially_destructible(T)> {
    };

    /** Equivalent of IsTriviallyDestructible<T>::Value. */
    template<typename T>
    inline constexpr bool IsTriviallyDestructibleV = IsTriviallyDestructible<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H