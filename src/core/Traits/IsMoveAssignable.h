#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_MOVE_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_MOVE_ASSIGNABLE_H0
#include "IsAssignable.h"
#include "AddLValueReference.h"
#include "AddRValueReference.h"
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether U is a move assignable type to T.
    * A move assignable type is a type that can be assigned an rvalue reference of the same type or another type
    * This includes scalar types and move assignable classes.
    * A move assignable class is a class that either has its move assignment operator defined (either the implicitly provided one or a custom defined one), or
    * a copy assignment that is called for rvalue references (these are always called unless the class has a deleted move assignment operator).
    */
    template<typename T, typename U = T>
    struct IsMoveAssignable
        : IsAssignable<AddLValueReferenceT<T>, AddRValueReferenceT<U>> {
    };

    /** Equivalent of IsMoveAssignable<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsMoveAssignableV = IsMoveAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_MOVE_ASSIGNABLE_H