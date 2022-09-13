#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE_H
#include "IsTriviallyConstructible.h"
#include "AddRValueReference.h"

namespace hud {

    /**
    * Checks whether U is a trivially move constructible type to T.
    * A trivially move constructible type is a type which can be trivially constructed from an rvalue reference of its type or another type
    * This includes scalar types, trivially move constructible classes and arrays of such types.
    * A trivially move constructible class is a class (defined with class, struct or union) that:
    *   - uses the implicitly defined move constructor.
    *   - has no virtual members.
    *   - its base class and non-static data members (if any) are themselves also trivially move constructible types.
    */
    template<typename T, typename U = T>
    struct IsTriviallyMoveConstructible
        : IsTriviallyConstructible < T, AddRValueReferenceT<U>> {
    };

    /** Equivalent of IsTriviallyMoveConstructible<T,U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsTriviallyMoveConstructibleV = IsTriviallyMoveConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE_H