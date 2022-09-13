#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_SWAPPABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_SWAPPABLE_H
#include "And.h"
#include "IsTriviallyDestructible.h"
#include "IsTriviallyMoveConstructible.h"
#include "IsTriviallyMoveAssignable.h"
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is trivially swappable with U.
    * A trivially swappable type are trivially destructible, trivially move constructible and trivially move assignable.
    */
    template<typename T, typename U = T>
    struct IsTriviallySwappable
        : And<IsTriviallyMoveConstructible<T, U>, IsTriviallyMoveAssignable<T, U>, IsTriviallyDestructible<T>, IsTriviallyDestructible<U>> {
    };

    /** Equivalent of IsTriviallySwappable<T, U>::Value */
    template<typename T, typename U = T>
    inline constexpr bool IsTriviallySwappableV = IsTriviallySwappable<T, U>::Value;

} // namespace hud 

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_SWAPPABLE_H