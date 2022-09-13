#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H
#include "IsConstructible.h"

namespace hud {

    /**
    * Checks whether T is a default constructible type.
    * A default constructible T is a type which can be constructed without arguments or initialization values, either cv-qualified or not.
    * This includes scalar types, default constructible classes and arrays of such types.
    * A default constructible class is a class that has a default constructor (either its implicit constructor or a custom defined one).
    */
    template<typename T>
    struct IsDefaultConstructible
        : IsConstructible<T> {
    };

    /** Equivalent of IsDefaultConstructible<T>::Value. */
    template<typename T>
    inline constexpr bool IsDefaultConstructibleV = IsDefaultConstructible<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H