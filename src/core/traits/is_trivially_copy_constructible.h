#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_COPY_CONSTRUCTIBLE_H
#include "is_trivially_constructible.h"
#include "add_const.h"
#include "add_lvalue_reference.h"

namespace hud
{

    /**
     * Checks whether u_type_t is a trivially copy constructible type to type_t.
     * A trivially copy constructible type is a type which can be trivially constructed from a value or reference of the same type or another type.
     * This includes scalar types, trivially copy constructible classes and arrays of such types.
     * A trivially copy constructible class is a class (defined with class, struct or union) that:
     *     - uses the implicitly defined copy constructor.
     *     - has no virtual members.
     *     - its base class and non-static data members (if any) are themselves also trivially copy constructible types.
     */
    template<typename type_t, typename u_type_t = type_t>
    struct is_trivially_copy_constructible
        : is_trivially_constructible<type_t, hud::add_lvalue_reference_t<hud::add_const_t<u_type_t>>>
    {
    };

    /** Equivalent of  is_trivially_copy_constructible<type_t,u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_COPY_CONSTRUCTIBLE_H
