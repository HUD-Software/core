#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPY_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPY_ASSIGNABLE_H
#include "IsTriviallyAssignable.h"
#include "AddLValueReference.h"
#include "AddConst.h"

namespace hud {

    /** Checks whether U is a trivially copy assignable type to T. */
    template<typename T, typename U = T>
    struct IsTriviallyCopyAssignable
        : IsTriviallyAssignable<AddLValueReferenceT<T>, AddLValueReferenceT<AddConstT<U>>> {
    };

    /** Equivalent of IsTriviallyCopyAssignable<T,U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsTriviallyCopyAssignableV = IsTriviallyCopyAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_COPY_ASSIGNABLE_H