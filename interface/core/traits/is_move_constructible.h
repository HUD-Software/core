#ifndef HD_INC_CORE_TRAITS_IS_MOVE_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_MOVE_CONSTRUCTIBLE_H
#include "is_constructible.h"
#include "add_rvalue_reference.h"
#include "integral_constant.h"

namespace hud
{

    /**
     * Checks whether u_type_t is a move constructible type to type_t.
     * A move constructible type is a type that can be constructed from an rvalue reference of its type or another type.
     * This includes scalar types and move constructible classes.
     * A move constructible class is a class that either has a move constructor (implicit or custom) or
     * a copy constructor that is called for rvalue references (these are always called unless the class has a deleted move constructor).
     * Note that this implies that all copy-constructible types are also move-constructible.
     */
    template<typename type_t, typename u_type_t = type_t>
    struct is_move_constructible
        : is_constructible<type_t, add_rvalue_reference_t<u_type_t>>
    {
    };

    /** Equivalent of hud::is_move_constructible_v<type_t,u_type_t>::value */
    template<typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_move_constructible_v = is_move_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_MOVE_CONSTRUCTIBLE_H