#ifndef HD_INC_CORE_TRAITS_IS_MOVE_ASSIGNABLE_H
#define HD_INC_CORE_TRAITS_IS_MOVE_ASSIGNABLE_H
#include "is_assignable.h"
#include "add_lvalue_reference.h"
#include "add_rvalue_reference.h"
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether u_type_t is a move assignable type to type_t.
     * A move assignable type is a type that can be assigned an rvalue reference of the same type or another type
     * This includes scalar types and move assignable classes.
     * A move assignable class is a class that either has its move assignment operator defined (either the implicitly provided one or a custom defined one), or
     * a copy assignment that is called for rvalue references (these are always called unless the class has a deleted move assignment operator).
     */
    template<typename type_t, typename u_type_t = type_t>
    struct is_move_assignable
        : is_assignable<hud::add_lvalue_reference_t<type_t>, add_rvalue_reference_t<u_type_t>>
    {
    };

    /** Equivalent of hud::is_move_assignable<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_move_assignable_v = hud::is_move_assignable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_MOVE_ASSIGNABLE_H