#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_DERIVABLE_FROM_H
#define HD_INC_OSLAYER_TRAITS_IS_DERIVABLE_FROM_H
#include "IsBaseOf.h"
#include "And.h"
#include "IsNotSame.h"
#include "RemoveCV.h"

namespace hud {

    /**
    * Checks whether Derived is a derived class of Base and not the same class, without regard to their const and/or volatile qualification.
    * Only classes that are not unions are considered.
    */
    template<typename Derived, typename Base>
    struct IsDerivedFrom
        : And<
        IsBaseOf<Base, Derived>,
        IsNotSame<RemoveCVT<Base>, RemoveCVT<Derived>>
        > {
    };

    /** Equivalent of IsDerivedFrom<Derived,Base>::Value. */
    template<typename Derived, typename Base>
    inline constexpr bool IsDerivedFromV = IsDerivedFrom<Derived, Base>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_DERIVABLE_FROM_H