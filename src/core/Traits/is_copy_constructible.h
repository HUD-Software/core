#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_COPY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_traits_IS_COPY_CONSTRUCTIBLE_H
#include "add_const.h"
#include "is_constructible.h"
#include "add_lvalue_reference.h"

namespace hud {

    /**
    * Checks whether U is a copy constructible type to T.
    * A copy constructible type is a type that can be constructed from a value or reference of the same type or another type.
    * A copy constructible class is a class that has a copy constructor (either its implicit constructor or a custom defined one)
    */
    template<typename T, typename U = T>
    struct IsCopyConstructible
        : IsConstructible<T, AddLValueReferenceT< AddConstT<U>>> {
    };

    /** Equivalent of IsCopyConstructible<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsCopyConstructibleV = IsCopyConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_COPY_CONSTRUCTIBLE_H