#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_COPYABLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_COPYABLE_H
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether type_t is a trivially copyable type.
     * A trivially copyable type is a type whose storage is contiguous (and thus its copy implies a trivial memory block copy, as if performed with memcpy), either cv-qualified or not.
     * This is true for scalar types, trivially copyable classes and arrays of any such types.
     * A trivially copyable class is a class (defined with class, struct or union) that:
     *     - uses the implicitly defined copy and move constructors, copy and move assignments, and destructor.
     *     - has no virtual members.
     *     - its base class and non-static data members (if any) are themselves also trivially copyable types.
     */
    template<typename type_t>
    struct is_trivially_copyable
        : hud::bool_constant<__is_trivially_copyable(type_t)>
    {
    };

    /** Equivalent of is_trivially_copyable<type_t>::value. */
    template<typename type_t>
    constexpr bool is_trivially_copyable_v = is_trivially_copyable<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_COPYABLE_H