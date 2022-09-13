#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_LESS_H
#define HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_LESS_H
#include "IntegralConstant.h"
#include "VoidT.h"
#include "../Templates/Declval.h"
#include "AddConst.h"
#include "AddLValueReference.h"

namespace hud {

    /** Checks whether Left type is comparable with Right type with operator<. */
    template<typename Left, typename Right, typename = void>
    struct IsComparableWithLess
        : FalseType {
    };

    template<typename Left, typename Right>
    struct IsComparableWithLess<Left, Right, VoidT<decltype(hud::declval<AddConstT<AddLValueReferenceT<Left>>>() < hud::declval<AddConstT<AddLValueReferenceT<Right>>>())>>
        : TrueType {
    };

    /** Equivalent of IsComparableWithLess<Left,Right>::Value. */
    template<typename Left, typename Right>
    inline constexpr bool IsComparableWithLessV = IsComparableWithLess<Left, Right>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_COMPARABLE_WITH_LESS_H