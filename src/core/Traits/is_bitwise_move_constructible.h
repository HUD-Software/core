#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_BITWISE_MOVE_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_traits_IS_BITWISE_MOVE_CONSTRUCTIBLE_H
#include "or.h"
#include "and.h"
#include "is_pointer.h"
#include "is_member_pointer.h"
#include "is_same_size.h"
#include "is_integral.h"
#include "is_enum.h"
#include "is_same.h"
#include "is_trivially_move_constructible.h"

namespace hud {

    /** Checks whether T is a bitwise move constructible type ( Constructible with a bitwise memory copy ) with the argument type U. */
    template<typename T, typename U>
    struct IsBitwiseMoveConstructible
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
        IsTriviallyMoveConstructible<T, U>
        >
        > {
    };

    /** Equivalent of IsBitwiseMoveConstructible<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsBitwiseMoveConstructibleV = IsBitwiseMoveConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_BITWISE_MOVE_CONSTRUCTIBLE_H
