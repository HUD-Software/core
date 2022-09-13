#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_BITWISE_CONVERTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_BITWISE_CONVERTIBLE_H
#include "Or.h"
#include "And.h"
#include "IsPointer.h"
#include "IsMemberPointer.h"
#include "IsSameSize.h"
#include "IsIntegral.h"
#include "IsEnum.h"
#include "IsTriviallyCopyable.h"

namespace hud {

    /** Checks whether T is a bitwise convertible type with the argument type U. */
    template<typename T, typename U>
    struct IsBitwiseConvertible
        : Or<
        And< Or<IsPointer<T>, IsMemberPointer<T>>, Or<IsPointer<U>, IsMemberPointer<U>>>, // If both are pointers or pointer's to member it's bitwise convertible
        And< Or< And<IsIntegral<T>, IsIntegral<U>>, And<IsEnum<T>, IsEnum<U>>>, IsSameSize<T, U>> // If same size and both are integral or enums it's bitwise convertible, assuming two's-complement
        > {
    };

    template<typename T>
    struct IsBitwiseConvertible<T, T>
        : IsTriviallyCopyable<T> {
    };


    /** Equivalent of IsBitwiseConvertible<T, U>::Value. */
    template<typename T, typename U>
    inline constexpr bool IsBitwiseConvertibleV = IsBitwiseConvertible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_BITWISE_CONVERTIBLE_H