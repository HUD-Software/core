#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H
#include "integral_constant.h"
#if !__has_builtin(__is_trivially_destructible)
    #include "is_destructible.h"
#endif

namespace hud
{

    /**
     * Checks whether type_t is a trivially destructible type.
     * Trivially destructible types include scalar types, trivially copy constructible classes and arrays of such types.
     * A trivially destructible class is a class (defined with class, struct or union) that:
     * - uses the implicitly defined destructor.
     * - the destructor is not virtual.
     * - its base class and non-static data members (if any) are themselves also trivially destructible types.
     */
    template<typename type_t>
    struct is_trivially_destructible
#if __has_builtin(__is_trivially_destructible)
        : hud::bool_constant<__is_trivially_destructible(type_t)>
#else
        : hud::conjunction<
              hud::is_destructible<type_t>,
              hud::bool_constant<__has_trivial_destructor(type_t)>>
#endif
    {
    };

    /** Equivalent of is_trivially_destructible<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_H