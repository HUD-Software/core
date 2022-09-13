#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_TRIVIAL_H
#define HD_INC_OSLAYER_TRAITS_IS_TRIVIAL_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is a trivial type.
    * A trivial type is a type whose storage is contiguous (trivially copyable) and which only supports static default initialization (trivially default constructible), either cv-qualified or not.
    * It includes scalar types, trivial classes and arrays of any such types.
    * A trivial class is a class (defined with class, struct or union) that is both trivially default constructible and trivially copyable, which implies that:
    *   - uses the implicitly defined default, copy and move constructors, copy and move assignments, and destructor.
    *   - has no virtual members.
    *   - has no non-static data members with brace- or equal- initializers.
    *   - its base class and non-static data members (if any) are themselves also trivial types.
    * Example:
    *   struct A { int m; };
    *   struct B { B() {} };
    *   IsTrivial<int>::Value -> true
    *   IsTrivial<A>::Value -> true
    *   IsTrivial<B>::Value -> false
    */
    template<typename T>
    struct IsTrivial
        : BoolConstant<__is_trivial(T)> {
    };

    /** Equivalent of IsTrivial<T>::Value */
    template<typename T>
    inline constexpr bool IsTrivialV = IsTrivial<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_TRIVIAL_H

