#ifndef HD_INC_CORE_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_H
#define HD_INC_CORE_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_H
#include "is_trivially_assignable.h"
#include "add_lvalue_reference.h"
#include "add_rvalue_reference.h"

namespace hud
{

    /** Checks whether u_type_t is a trivially move assignable type to type_t. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_trivially_move_assignable
        : is_trivially_assignable<hud::add_lvalue_reference_t<type_t>, add_rvalue_reference_t<u_type_t>>
    {
    };

    /** Equivalent of is_trivially_move_assignable<type_t, u_type_t>::value */
    template<typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_H