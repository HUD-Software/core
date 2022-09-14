#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_NOTHROW_SWAPPABLE_H
#define HD_INC_OSLAYER_traits_IS_NOTHROW_SWAPPABLE_H
#include "integral_constant.h"
#include "is_swappable.h"
#include "and.h"

namespace hud {

    namespace details {
        template<typename T, typename U = T>
        struct IsNothrowSwappableImpl
            : BoolConstant<noexcept(swap(declval<T>(), declval<U>())) && noexcept(swap(declval<T>(), declval<U>()))> {
        };
    }

    /** Check whether T is swappable with U and such swap is known not to throw any exception. */
    template<typename T, typename U = T>
    struct IsNothrowSwappable
        : And< IsSwappable<T, U>, details::IsNothrowSwappableImpl<AddLValueReferenceT<T>, AddLValueReferenceT<U>>> {
    };

    /** Equivalent of IsNothrowSwappable<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsNothrowSwappableV = IsNothrowSwappable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_NOTHROW_SWAPPABLE_H