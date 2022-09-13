#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_H
#include "IsNothrowAssignable.h"
#include "AddLValueReference.h"
#include "AddRValueReference.h"

namespace hud {

    /** Checks whether T is move assignable type from U, and such construction is known not to throw any exception. */
    template <typename T, typename U = T>
    struct IsNothrowMoveAssignable
        : IsNothrowAssignable<AddLValueReferenceT<T>, AddRValueReferenceT<U>> {
    };

    /** Equivalent of IsNothrowMoveAssignable<T,U>::Value. */
    template <typename T, typename U = T>
    inline constexpr bool IsNothrowMoveAssignableV = IsNothrowMoveAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_H