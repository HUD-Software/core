#ifndef HD_INC_CORE_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#include "add_lvalue_reference.h"
#include "add_const.h"
#include "is_explicitly_constructible.h"

namespace hud {

    /** Checks whether type_t is an explicitly copy constructible type with u_type_t. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_explicitly_copy_constructible
        : is_explicitly_constructible< type_t, hud::add_lvalue_reference_t<hud::add_const_t<u_type_t>> > {
    };

    /** Equivalent of hud::is_explicitly_copy_constructible<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_explicitly_copy_constructible_v = hud::is_explicitly_copy_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H