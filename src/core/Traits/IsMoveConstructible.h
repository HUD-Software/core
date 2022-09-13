#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_MOVE_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_MOVE_CONSTRUCTIBLE_H
#include "IsConstructible.h"
#include "AddRValueReference.h"
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether U is a move constructible type to T.
    * A move constructible type is a type that can be constructed from an rvalue reference of its type or another type.
    * This includes scalar types and move constructible classes.
    * A move constructible class is a class that either has a move constructor (implicit or custom) or
    * a copy constructor that is called for rvalue references (these are always called unless the class has a deleted move constructor).
    * Note that this implies that all copy-constructible types are also move-constructible.
    */
    template<typename T, typename U = T >
    struct IsMoveConstructible
        : IsConstructible < T, AddRValueReferenceT<U>> {
    };

    /** Equivalent of IsMoveConstructibleV<T,U>::Value */
    template<typename T, typename U = T >
    inline constexpr bool IsMoveConstructibleV = IsMoveConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_MOVE_CONSTRUCTIBLE_H