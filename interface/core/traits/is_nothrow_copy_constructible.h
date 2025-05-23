#ifndef HD_INC_CORE_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_H
#include "is_nothrow_constructible.h"
#include "add_lvalue_reference.h"
#include "add_const.h"

namespace hud
{

    /** Checks whether type_t is copy constructible type from u_type_t, and such construction is known not to throw any exception. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_nothrow_copy_constructible
        : is_nothrow_constructible<type_t, hud::add_lvalue_reference_t<hud::add_const_t<u_type_t>>>
    {
    };

    /** Equivalent of is_nothrow_copy_constructible<type_t,u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_H