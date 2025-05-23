#ifndef HD_INC_CORE_TRAITS_IS_BITWISE_COPYABLE_H
#define HD_INC_CORE_TRAITS_IS_BITWISE_COPYABLE_H
#include "is_bitwise_convertible.h"
#include "is_trivially_copyable.h"

namespace hud
{

    /** Checks whether type_t is a bitwise copyable type with the argument type u_type_t. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_bitwise_copyable
        : hud::is_bitwise_convertible<type_t, u_type_t>
    {
    };

    template<typename type_t>
    struct is_bitwise_copyable<type_t, type_t>
        : is_trivially_copyable<type_t>
    {
    };

    /** Equivalent of is_bitwise_copyable<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_bitwise_copyable_v = is_bitwise_copyable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BITWISE_COPYABLE_H