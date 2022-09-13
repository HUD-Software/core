#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_H
#include "IsTriviallyAssignable.h"
#include "AddLValueReference.h"
#include "AddRValueReference.h"

namespace hud {

    /** Checks whether U is a trivially move assignable type to T. */
    template<typename T, typename U = T>
    struct IsTriviallyMoveAssignable
        : IsTriviallyAssignable<AddLValueReferenceT<T>, AddRValueReferenceT<U>> {
    };

    /** Equivalent of IsTriviallyMoveAssignable<T, U>::Value */
    template<typename T, typename U = T>
    inline constexpr bool IsTriviallyMoveAssignableV = IsTriviallyMoveAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_H