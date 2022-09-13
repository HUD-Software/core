#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPY_CONSTRUCTIBLE_H
#include "IsTriviallyConstructible.h"
#include "AddConst.h"
#include "AddLValueReference.h"

namespace hud {

    /**
    * Checks whether U is a trivially copy constructible type to T.
    * A trivially copy constructible type is a type which can be trivially constructed from a value or reference of the same type or another type.
    * This includes scalar types, trivially copy constructible classes and arrays of such types.
    * A trivially copy constructible class is a class (defined with class, struct or union) that:
    *     - uses the implicitly defined copy constructor.
    *     - has no virtual members.
    *     - its base class and non-static data members (if any) are themselves also trivially copy constructible types.
    */
    template<typename T, typename U = T>
    struct IsTriviallyCopyConstructible
        : IsTriviallyConstructible< T, AddLValueReferenceT< AddConstT<U>>> {
    };

    /** Equivalent of  IsTriviallyCopyConstructible<T,U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsTriviallyCopyConstructibleV = IsTriviallyCopyConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPY_CONSTRUCTIBLE_H
