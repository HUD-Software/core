#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_TRIVIALLY_MOVE_ASSIGNABLE_H
#define HD_INC_OSLAYER_traits_IS_TRIVIALLY_MOVE_ASSIGNABLE_H
#include "is_trivially_assignable.h"
#include "add_lvalue_reference.h"
#include "add_rvalue_reference.h"

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

#endif // HD_INC_OSLAYER_traits_IS_TRIVIALLY_MOVE_ASSIGNABLE_H