#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_NOTHROW_COPY_ASSIGNABLE_H
#define HD_INC_OSLAYER_traits_IS_NOTHROW_COPY_ASSIGNABLE_H
#include "is_nothrow_assignable.h"
#include "add_lvalue_reference.h"
#include "add_const.h"

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

#endif // HD_INC_OSLAYER_traits_IS_NOTHROW_COPY_ASSIGNABLE_H