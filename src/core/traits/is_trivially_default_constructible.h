#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE_H
#include "is_trivially_constructible.h"

namespace hud
{

    /**
     * Checks whether type_t is a trivially default constructible type.
     * A trivially default constructible type is a type which can be trivially constructed without arguments or initialization values, either cv-qualified or not.
     * This includes scalar types, trivially default constructible classes and arrays of such types.
     * A trivially default constructible class is a class (defined with class, struct or union) that:
     * - uses the implicitly defined default constructor.
     * - has no virtual members.
     * - has no non-static data members with brace- or equal- initializers.
     * - its base class and non-static data members (if any) are themselves also trivially default constructible types.
     */
    template <typename type_t>
    struct is_trivially_default_constructible
        : is_trivially_constructible<type_t>
    {
    };

    /** Equivalent of is_trivially_default_constructible<type_t>::value. */
    template <typename type_t>
    inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE_H