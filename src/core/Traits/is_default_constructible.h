#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H
#include "is_constructible.h"

namespace hud {

    /**
    * Checks whether type_t is a default constructible type.
    * A default constructible type_t is a type which can be constructed without arguments or initialization values, either cv-qualified or not.
    * This includes scalar types, default constructible classes and arrays of such types.
    * A default constructible class is a class that has a default constructor (either its implicit constructor or a custom defined one).
    */
    template<typename type_t>
    struct is_default_constructible
        : is_constructible<type_t> {
    };

    /** Equivalent of is_default_constructible<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_default_constructible_v = is_default_constructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H