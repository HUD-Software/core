#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#include "IsImplicitlyConstructible.h"

namespace hud {

    /** Checks whether T is an implicitly default constructible type. */
    template <typename T>
    struct IsImplicitlyDefaultConstructible
        : IsImplicitlyConstructible<T> {
    };

    /** Equivalent of IsImplicitlyDefaultConstructible<T>::Value */
    template<typename T>
    inline constexpr bool IsImplicitlyDefaultConstructibleV = IsImplicitlyDefaultConstructible<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H