#ifndef HD_INC_CORE_TRAITS_IS_TRANSPARENT_H
#define HD_INC_CORE_TRAITS_IS_TRANSPARENT_H
#include "integral_constant.h"
#include "void_t.h"

namespace hud
{
    /**
     * Check if the type_t is transparent.
     * A transparent type_t is a type that define an inner type called 'is_transparent' (mainly a using statement)
     * that indicate that the type_t as the ability to search in an associative containers
     * with something that is semantically a key, even if it is not technically a key.
     * For exemple a `hasher<string>` type_t can hash a string and a string_view, it should be marked `is_transparent`
     * to allow the string_view to be used as key in find/insert of the associative container.
     */
    template<typename type_t, typename = void>
    struct is_transparent
        : hud::false_type
    {
    };

    template<class T>
    struct is_transparent<T, hud::void_t<typename T::is_transparent>>
        : hud::true_type
    {
    };

    /** Equivalent of is_transparent<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_transparent_v = is_transparent<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRANSPARENT_H