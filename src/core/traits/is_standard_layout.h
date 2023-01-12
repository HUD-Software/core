#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_STANDARD_LAYOUT_H
#define HD_INC_CORE_TRAITS_IS_STANDARD_LAYOUT_H
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether type_t is a standard-layout type.
     * A standard-layout type is a type with a simple linear data structure and access control that can easily be used to communicate with code written in other programming languages, such as C, either cv-qualified or not.
     * This is true for scalar types, standard-layout classes and arrays of any such types.
     * A standard-layout class is a class (defined with class, struct or union) that:
     *    - has no virtual functions and no virtual base classes.
     *    - has the same access control (private, protected, public) for all its non-static data members.
     *    - either has no non-static data members in the most derived class and at most one base class with non-static data members, or has no base classes with non-static data members.
     *    - its base class (if any) is itself also a standard-layout class. and,
     *    - has no base classes of the same type as its first non-static data member.
     * Example:
     *   struct A { int m; };
     *   struct B { int m1; private: int m2; };
     *   struct C { virtual void foo(); };
     *   is_standard_layout<int>::value -> true
     *   is_standard_layout<A>::value -> true
     *   is_standard_layout<B>::value -> false
     *   is_standard_layout<C>::value -> false
     */
    template <typename type_t>
    struct is_standard_layout
        : hud::bool_constant<__is_standard_layout(type_t)>
    {
    };

    /** Equivalent of is_standard_layout<type_t>::value. */
    template <typename type_t>
    inline constexpr bool is_standard_layout_v = is_standard_layout<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_STANDARD_LAYOUT_H
