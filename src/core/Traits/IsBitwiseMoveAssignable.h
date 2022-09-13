#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_BITWISE_MOVE_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_BITWISE_MOVE_ASSIGNABLE_H
#include "Or.h"
#include "And.h"
#include "IsPointer.h"
#include "IsMemberPointer.h"
#include "IsSameSize.h"
#include "IsIntegral.h"
#include "IsEnum.h"
#include "IsSame.h"
#include "IsTriviallyMoveAssignable.h"

namespace hud {

    /** Checks whether T is a bitwise move assignable type ( Constructible with a bitwise memory copy ) with the argument type U. */
    template<typename T, typename U>
    struct IsBitwiseMoveAssignable
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
        And< // Or if same type trivially move assignable
        IsSame<T, U>,
        IsTriviallyMoveAssignable<T, U>
        >
        > {
    };

    /** Equivalent of IsBitwiseMoveAssignable<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsBitwiseMoveAssignableV = IsBitwiseMoveAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BITWISE_MOVE_ASSIGNABLE_H