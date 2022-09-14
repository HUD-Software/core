#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_COMPARABLE_WITH_NOT_EQUAL_H
#define HD_INC_OSLAYER_traits_IS_COMPARABLE_WITH_NOT_EQUAL_H
#include "integral_constant.h"
#include "void_t.h"
#include "../templates/declval.h"
#include "add_const.h"
#include "add_lvalue_reference.h"

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

#endif // HD_INC_OSLAYER_traits_IS_COMPARABLE_WITH_NOT_EQUAL_H