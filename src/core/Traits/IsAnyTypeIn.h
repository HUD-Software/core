#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_ANY_TYPE_IN_H
#define HD_INC_OSLAYER_TRAITS_IS_ANY_TYPE_IN_H
#include "Or.h"
#include "IsSame.h"

namespace hud {

    /** Check if the type Find is in List of types. */
    template<typename Find, typename... List>
    struct IsAnyTypeIn
        : Or<IsSame<Find, List>...> {
    };

    /** Equivalent of IsAnyTypeIn<Find, List...>::Value. */
    template<typename Find, typename... List>
    inline constexpr bool IsAnyTypeInV = IsAnyTypeIn<Find, List...>::Value;
}

#endif // HD_INC_OSLAYER_TRAITS_IS_ANY_TYPE_IN_H
