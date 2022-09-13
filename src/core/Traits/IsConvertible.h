#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_CONVERTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_CONVERTIBLE_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether From is implicitly convertible to To. */
    template <typename From, typename To>
    struct IsConvertible
        : BoolConstant<__is_convertible_to(From, To)> {}
    ;

    /** Equivalent of IsConvertible<From,To>::Value. */
    template <typename From, typename To>
    inline constexpr bool IsConvertibleV = IsConvertible<From, To>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_CONVERTIBLE_H