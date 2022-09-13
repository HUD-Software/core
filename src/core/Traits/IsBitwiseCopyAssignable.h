#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_BITWISE_COPY_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_BITWISE_COPY_ASSIGNABLE_H
#include "Or.h"
#include "And.h"
#include "IsPointer.h"
#include "IsMemberPointer.h"
#include "IsSameSize.h"
#include "IsIntegral.h"
#include "IsEnum.h"
#include "IsSame.h"
#include "IsTriviallyCopyAssignable.h"

namespace hud {

    /** Checks whether T is a bitwise copy assignable type ( Constructible with a bitwise memory copy ) with the argument type U. */
    template<typename T, typename U>
    struct IsBitwiseCopyAssignable
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
        And< // Or if same type trivially copy assignable
        IsSame<T, U>,
        IsTriviallyCopyAssignable<T, U>
        >
        > {
    };

    /** Equivalent of IsBitwiseCopyAssignable<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsBitwiseCopyAssignableV = IsBitwiseCopyAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BITWISE_COPY_ASSIGNABLE_H