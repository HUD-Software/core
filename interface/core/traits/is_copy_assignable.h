#ifndef HD_INC_CORE_TRAITS_IS_COPY_ASSIGNABLE_H
#define HD_INC_CORE_TRAITS_IS_COPY_ASSIGNABLE_H
#include "is_assignable.h"
#include "add_lvalue_reference.h"
#include "add_const.h"

namespace hud
{

    /**
     * Checks whether u_type_t is a copy assignable type to type_t.
     * A copy assignable type is a type that can be assigned a value or reference of the same type or another type.
     * This includes scalar types and copy assignable classes.
     * A copy assignable class is a class that has its copy assignment operator defined (either the implicitly provided one or a custom defined one).
     */
    template<typename type_t, typename u_type_t = type_t>
    struct is_copy_assignable
        : is_assignable<hud::add_lvalue_reference_t<type_t>, hud::add_lvalue_reference_t<hud::add_const_t<u_type_t>>>
    {
    };

    /** Equivalent of hud::is_copy_assignable<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_copy_assignable_v = hud::is_copy_assignable<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_COPY_ASSIGNABLE_H