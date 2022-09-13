#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is a trivially constructible type with the set of argument types specified by Arg.
    * Trivially constructible T is a type which can be trivially default-, copy- or move- constructed, depending on the particular set of argument types used as TArgs
    */
    template<typename T, typename... TArgs>
    struct IsTriviallyConstructible
        : BoolConstant<__is_trivially_constructible(T, TArgs...)> {
    };

    /** Equivalent of IsTriviallyConstructible<T, TArgs...>::Value. */
    template<typename T, typename... TArgs>
    inline constexpr bool IsTriviallyConstructibleV = IsTriviallyConstructible<T, TArgs...>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIALLY_CONSTRUCTIBLE_H