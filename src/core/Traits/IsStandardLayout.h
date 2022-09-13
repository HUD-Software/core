#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_STANDARD_LAYOUT_H
#define HD_INC_OSLAYER_TRAITS_IS_STANDARD_LAYOUT_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Checks whether T is a standard-layout type.
    * A standard-layout type is a type with a simple linear data structure and access control that can easily be used to communicate with code written in other programming languages, such as C, either cv-qualified or not.
    * This is true for scalar types, standard-layout classes and arrays of any such types.
    * A standard-layout class is a class (defined with class, struct or union) that:
    *    - has no virtual functions and no virtual base classes.
    *    - has the same access control (private, protected, public) for all its non-static data members.
    *    - either has no non-static data members in the most derived class and at most one base class with non-static data members, or has no base classes with non-static data members.
    *    - its base class (if any) is itself also a standard-layout class. And,
    *    - has no base classes of the same type as its first non-static data member.
    * Example:
    *   struct A { int m; };
    *   struct B { int m1; private: int m2; };
    *   struct C { virtual void foo(); };
    *   IsStandardLayout<int>::Value -> true
    *   IsStandardLayout<A>::Value -> true
    *   IsStandardLayout<B>::Value -> false
    *   IsStandardLayout<C>::Value -> false
    */
    template<typename T>
    struct IsStandardLayout
        : BoolConstant<__is_standard_layout(T)> {
    };

    /** Equivalent of IsStandardLayout<T>::Value. */
    template<typename T>
    inline constexpr bool IsStandardLayoutV = IsStandardLayout<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_STANDARD_LAYOUT_H
