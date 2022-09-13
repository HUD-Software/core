#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_H
#include "IsNothrowConstructible.h"
#include "AddLValueReference.h"
#include "AddConst.h"

namespace hud {

    /** Checks whether T is copy constructible type from U, and such construction is known not to throw any exception. */
    template <typename T, typename U = T>
    struct IsNothrowCopyConstructible
        : IsNothrowConstructible<T, AddLValueReferenceT<AddConstT<U>>> {
    };

    /** Equivalent of IsNothrowCopyConstructible<T,U>::Value. */
    template <typename T, typename U = T>
    inline constexpr bool IsNothrowCopyConstructibleV = IsNothrowCopyConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_H