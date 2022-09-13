#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_NOT_EQUAL_H
#define HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_NOT_EQUAL_H
#include "IntegralConstant.h"
#include "VoidT.h"
#include "../Templates/Declval.h"
#include "AddConst.h"
#include "AddLValueReference.h"

namespace hud {

    /** Checks whether Left type is comparable with Right type with operator!=. */
    template<typename Left, typename Right, typename = void>
    struct IsComparableWithNotEqual
        : FalseType {
    };

    template<typename Left, typename Right>
    struct IsComparableWithNotEqual<Left, Right, VoidT<decltype(hud::declval<AddConstT<AddLValueReferenceT<Left>>>() != hud::declval< AddConstT < AddLValueReferenceT<Right>>>())>>
        : TrueType {
    };

    /** Equivalent of IsComparableWithNotEqual<Left,Right>::Value. */
    template<typename Left, typename Right>
    inline constexpr bool IsComparableWithNotEqualV = IsComparableWithNotEqual<Left, Right>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_NOT_EQUAL_H