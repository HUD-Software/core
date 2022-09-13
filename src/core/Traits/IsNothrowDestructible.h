#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_NOTHROW_DESTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_NOTHROW_DESTRUCTIBLE_H
#include "IntegralConstant.h"
#include "../Templates/Declval.h"
#include "And.h"
#include "IsDestructible.h"

namespace hud {
/**
    * Checks whether T is a destructible type, and such destruction is known not to throw any exception.
    * Notice that all class destructors are noexcept unless explicitly specified otherwise.
    */
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    template <typename T>
    struct IsNothrowDestructible
        : BoolConstant<__is_nothrow_destructible(T)> {
    };
#else
    namespace details {
        template<typename T>
        struct IsNothrowDestructibleWellFormed
            : BoolConstant<noexcept(hud::declval<T>().~T())> {
        };
    }

    template <typename T>
    struct IsNothrowDestructible
        : And<hud::IsDestructible<T>,details::IsNothrowDestructibleWellFormed<T>> {
    };

#endif
    /** Equivalent of IsNothrowDestructible<T>::Value. */
    template <class T>
    inline constexpr bool IsNothrowDestructibleV = IsNothrowDestructible<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_NOTHROW_DESTRUCTIBLE_H

