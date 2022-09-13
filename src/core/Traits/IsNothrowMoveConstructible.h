#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_H
#include "IsNothrowConstructible.h"
#include "AddRValueReference.h"

namespace hud {

    /** Checks whether T is move constructible type from U, and such construction is known not to throw any exception. */
    template <typename T, typename U = T>
    struct IsNothrowMoveConstructible
        : IsNothrowConstructible<T, AddRValueReferenceT<U>> {
    };

    /** Checks whether U is move constructible type from V, and such construction is known not to throw any exception. */
    template <typename T, typename U = T>
    constexpr bool IsNothrowMoveConstructibleV = IsNothrowMoveConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_H
