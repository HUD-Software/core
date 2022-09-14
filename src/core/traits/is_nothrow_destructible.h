#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_DESTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_DESTRUCTIBLE_H
#include "integral_constant.h"
#include "../templates/declval.h"
#include "conjunction.h"
#include "is_destructible.h"

namespace hud {
/**
    * Checks whether type_t is a destructible type, and such destruction is known not to throw any exception.
    * Notice that all class destructors are noexcept unless explicitly specified otherwise.
    */
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    template <typename type_t>
    struct is_nothrow_destructible
        : bool_constant<__is_nothrow_destructible(type_t)> {
    };
#else
    namespace details {
        template<typename type_t>
        struct IsNothrowDestructibleWellFormed
            : bool_constant<noexcept(hud::declval<type_t>().~type_t())> {
        };
    }

    template <typename type_t>
    struct is_nothrow_destructible
        : conjunction<hud::is_destructible<type_t>,details::IsNothrowDestructibleWellFormed<type_t>> {
    };

#endif
    /** Equivalent of is_nothrow_destructible<type_t>::value. */
    template <class type_t>
    inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOTHROW_DESTRUCTIBLE_H

