#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#include "IsExplicitlyConstructible.h"

namespace hud {

    /** Checks whether T is an explicitly default constructible type. */
    template <typename T>
    struct IsExplicitlyDefaultConstructible
        : public IsExplicitlyConstructible<T> {
    };

    /** Equivalent of IsExplicitlyDefaultConstructible<T>::Value. */
    template<typename T>
    inline constexpr bool IsExplicitlyDefaultConstructibleV = IsExplicitlyDefaultConstructible<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H