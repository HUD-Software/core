#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_BITWISE_COMPARABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_BITWISE_COMPARABLE_H
#include "Or.h"
#include "And.h"
#include "IsPointer.h"
#include "IsMemberPointer.h"
#include "IsSameSize.h"
#include "IsIntegral.h"
#include "IsEnum.h"
#include "IsSame.h"
#include "HasUniqueObjectRepresentations.h"
#include "IsStandardLayout.h"

namespace hud {

    /**
    * Checks whether T is a bitwise comparable with U.
    * There is no good way to detect this, specify this trait for your type to allow bitwise comparison.
    * This traits is specialize with Value equal to true if:
    * - T and U are pointers or pointers to member function
    * - T and U are integrals with same size
    * - T and U are enums with underlying type with same size
    */
    template<typename T, typename U = T>
    struct IsBitwiseComparable
        : Or<
        And<  // If both are pointers or pointer's to member it's bitwise comparable
        Or<IsPointer<T>, IsMemberPointer<T>>,
        Or<IsPointer<U>, IsMemberPointer<U>>
        >,
        And< // Or if same size and both are integral or enums it's bitwise comparable
        IsSameSize<T, U>,
        Or<
        And<IsIntegral<T>, IsIntegral<U>>, // Assuming two's-complement
        And<IsEnum<T>, IsEnum<U>>
        >
        >,
        And< // Or if same POD type. POD 
        IsSame<T, U>,
        And<HasUniqueObjectRepresentations<T>>,
        And<IsStandardLayout<T>>
        >
        > {
    };

    /** Equivalent of IsBitwiseComparable<U, V=U>::Value. */
    template<typename U, typename V = U>
    inline constexpr bool IsBitwiseComparableV = IsBitwiseComparable<U, V>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BITWISE_COMPARABLE_H
