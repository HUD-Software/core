#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_UNION_H
#define HD_INC_OSLAYER_TRAITS_IS_UNION_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a union type. */
    template<typename T>
    struct IsUnion
        : BoolConstant<__is_union(T)> {
    };

    /** Equivalent of IsUnion<T>::Value */
    template<typename T>
    inline constexpr bool IsUnionV = IsUnion<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_UNION_H