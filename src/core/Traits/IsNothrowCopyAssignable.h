#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_COPY_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_COPY_ASSIGNABLE_H
#include "IsNothrowAssignable.h"
#include "AddLValueReference.h"
#include "AddConst.h"

namespace hud {

    /** Checks whether T is copy assignable type from U, and such construction is known not to throw any exception. */
    template <typename T, typename U = T>
    struct IsNothrowCopyAssignable
        : IsNothrowAssignable<AddLValueReferenceT<T>, AddLValueReferenceT<AddConstT<U>>> {
    };

    /** Equivalent of IsNothrowCopyAssignable<T, U = T>::Value. */
    template <typename T, typename U = T>
    inline constexpr bool IsNothrowCopyAssignableV = IsNothrowCopyAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NOTHROW_COPY_ASSIGNABLE_H