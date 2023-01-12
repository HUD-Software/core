#pragma once
#ifndef HD_INC_CORE_TEMPLATES_SWAP_H
#define HD_INC_CORE_TEMPLATES_SWAP_H
#include "move.h"
#include "../traits/is_move_constructible.h"
#include "../traits/is_move_assignable.h"
#include "../traits/is_nothrow_move_constructible.h"
#include "../traits/is_nothrow_move_assignable.h"
#include "../traits/is_bitwise_move_constructible.h"
#include "../traits/is_bitwise_copy_constructible.h"
#include "../traits/is_default_constructible.h"

namespace hud
{

    /** Swap 2 a and b. a and b must move constructible and move assignable. */
    template <typename type_t, typename u_type_t = type_t>
    static constexpr void swap(type_t &a, u_type_t &b) noexcept
        requires(hud::is_move_constructible_v<type_t, u_type_t> && hud::is_move_assignable_v<u_type_t, type_t>)
    {
        static_assert(hud::is_nothrow_move_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) or type_t(u_type_t&&) is throwable. swap is not designed to allow throwable constructible components");
        static_assert(hud::is_nothrow_move_assignable_v<type_t, u_type_t>, "type_t& type_t::operator=(const u_type_t&) or type_t& type_t::operator=(u_type_t&&) is throwable. swap is not designed to allow throwable assignable components");
        type_t tmp = hud::move(a);
        a = hud::move(b);
        b = hud::move(tmp);
    }

    /** Swap 2 a and b array. a and b must move constructible and move assignable. */
    template <typename type_t, usize N>
    static constexpr void swap(type_t (&a)[N], type_t (&b)[N]) noexcept
        requires(hud::is_default_constructible_v<type_t> && hud::is_move_constructible_v<type_t> && hud::is_move_assignable_v<type_t>)
    {
        static_assert(hud::is_nothrow_move_constructible_v<type_t>, "type_t(type_t&&) is throwable. swap is not designed to allow throwable move constructible components");
        static_assert(hud::is_nothrow_move_assignable_v<type_t>, "type_t& type_t::operator=(type_t&&) is throwable. swap is not designed to allow throwable move assignable components");
        type_t tmp[N];
        if constexpr (is_bitwise_move_constructible_v<type_t> && is_bitwise_copy_constructible_v<type_t>)
        {
            hud::memory::template copy_construct_array(tmp, a, N);
            hud::memory::template copy_assign_array(a, b, N);
            hud::memory::template copy_assign_array(b, tmp, N);
        }
        else
        {
            hud::memory::template move_or_copy_construct_array(tmp, a, N);
            hud::memory::template move_or_copy_assign_array(a, b, b + N);
            hud::memory::template move_or_copy_assign_array(b, tmp, tmp + N);
        }
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_SWAP_H
