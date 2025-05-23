#ifndef HD_INC_CORE_TRAITS_IS_TRIVIAL_H
#define HD_INC_CORE_TRAITS_IS_TRIVIAL_H
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether type_t is a trivial type.
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
     *   is_trivial<int>::value -> true
     *   is_trivial<A>::value -> true
     *   is_trivial<B>::value -> false
     */
    template<typename type_t>
    struct is_trivial
        : hud::bool_constant<__is_trivial(type_t)>
    {
    };

    /** Equivalent of is_trivial<type_t>::value */
    template<typename type_t>
    constexpr bool is_trivial_v = is_trivial<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIAL_H
