#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_LESS_EQUAL_H
#define HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_LESS_EQUAL_H
#include "IntegralConstant.h"
#include "VoidT.h"
#include "../Templates/Declval.h"
#include "AddConst.h"
#include "AddLValueReference.h"

namespace hud {

    /** Checks whether Left type is comparable with Right type with operator<=. */
    template<typename Left, typename Right, typename = void>
    struct IsComparableWithLessEqual
        : FalseType {
    };

    template<typename Left, typename Right>
    struct IsComparableWithLessEqual<Left, Right, VoidT<decltype(hud::declval<AddConstT<AddLValueReferenceT<Left>>>() <= hud::declval<AddConstT<AddLValueReferenceT<Right>>>())>>
        : TrueType {
    };

    /** Equivalent of IsComparableWithLessEqual<Left,Right>::Value. */
    template<typename Left, typename Right>
    inline constexpr bool IsComparableWithLessEqualV = IsComparableWithLessEqual<Left, Right>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_LESS_EQUAL_H