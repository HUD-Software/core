#pragma once
#ifndef HD_INC_OSLAYER_templates_SWAP_H
#define HD_INC_OSLAYER_templates_SWAP_H
#include "move.h"
#include "../traits/is_move_constructible.h"
#include "../traits/is_move_assignable.h"
#include "../traits/is_nothrow_move_constructible.h"
#include "../traits/is_nothrow_move_assignable.h"
#include "../traits/is_bitwise_move_constructible.h"
#include "../traits/is_bitwise_copy_constructible.h"
#include "../traits/is_default_constructible.h"

namespace hud {

    /** Swap 2 a and b. a and b must move constructible and move assignable. */
    template<typename T, typename U = T>
    static constexpr void swap(T& a, U& b) noexcept requires(IsMoveConstructibleV<T, U>&& IsMoveAssignableV<U, T>) {
        static_assert(IsNothrowMoveConstructibleV<T, U>, "T(const U&) or T(U&&) is throwable. swap is not designed to allow throwable constructible components");
        static_assert(IsNothrowMoveAssignableV<T, U>, "T& T::operator=(const U&) or T& T::operator=(U&&) is throwable. swap is not designed to allow throwable assignable components");
        T tmp = hud::move(a);
        a = hud::move(b);
        b = hud::move(tmp);
    }

    /** Swap 2 a and b array. a and b must move constructible and move assignable. */
    template<typename T, usize N>
    static constexpr void swap(T(&a)[N], T(&b)[N]) noexcept requires( IsDefaultConstructibleV<T> && IsMoveConstructibleV<T>&& IsMoveAssignableV<T>) {
        static_assert(IsNothrowMoveConstructibleV<T>, "T(T&&) is throwable. swap is not designed to allow throwable move constructible components");
        static_assert(IsNothrowMoveAssignableV<T>, "T& T::operator=(T&&) is throwable. swap is not designed to allow throwable move assignable components");
        T tmp[N];
        if constexpr (IsBitwiseMoveConstructibleV<T> && IsBitwiseCopyConstructibleV<T>) {
            Memory::template copy_construct_array(tmp, a, N);
            Memory::template copy_assign_array(a, b, N);
            Memory::template copy_assign_array(b, tmp, N);
        }
        else {
            Memory::template move_or_copy_construct_array(tmp, a, N);
            Memory::template move_or_copy_assign_array(a, b, b+N);
            Memory::template move_or_copy_assign_array(b, tmp, tmp+N);
        }
    }

} // namespace hud

#endif // HD_INC_OSLAYER_templates_SWAP_H
