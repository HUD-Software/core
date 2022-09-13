#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPYABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPYABLE_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is a trivially copyable type.
    * A trivially copyable type is a type whose storage is contiguous (and thus its copy implies a trivial memory block copy, as if performed with memcpy), either cv-qualified or not.
    * This is true for scalar types, trivially copyable classes and arrays of any such types.
    * A trivially copyable class is a class (defined with class, struct or union) that:
    *     - uses the implicitly defined copy and move constructors, copy and move assignments, and destructor.
    *     - has no virtual members.
    *     - its base class and non-static data members (if any) are themselves also trivially copyable types.
    */
    template <typename T>
    struct IsTriviallyCopyable
        : BoolConstant<__is_trivially_copyable(T)> {
    };

    /** Equivalent of IsTriviallyCopyable<T>::Value. */
    template <typename T>
    inline constexpr bool IsTriviallyCopyableV = IsTriviallyCopyable<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPYABLE_H