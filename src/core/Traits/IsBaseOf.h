#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_BASE_OF_H
#define HD_INC_OSLAYER_TRAITS_IS_BASE_OF_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether Base is a base class of (or the same class as) Derived, without regard to their const and/or volatile qualification.
    * Only classes that are not unions are considered.
    */
    template<typename Base, typename Derived>
    struct IsBaseOf
        : BoolConstant<__is_base_of(Base, Derived)> {
    };

    /** Equivalent of IsBaseOf<Base, Derived>::Value. */
    template<typename Base, typename Derived>
    inline constexpr bool IsBaseOfV = IsBaseOf<Base, Derived>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BASE_OF_H