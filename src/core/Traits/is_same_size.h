#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_SAME_SIZE_H
#define HD_INC_CORE_TRAITS_IS_SAME_SIZE_H
#include "integral_constant.h"
#include "remove_all_extents.h"

namespace hud {

    /** Checks whether RemoveAllExtentT<type_t> is the same size has RemoveAllExtentT<u_type_t>. */
    template<typename type_t, typename u_type_t>
    struct is_same_size
        : bool_constant<sizeof(remove_all_extents_t<type_t>) == sizeof(remove_all_extents_t<u_type_t>)> {
    };

    /** Equivalent of is_same_size<type_t,u_type_t>::value. */
    template<typename type_t, typename u_type_t >
    inline constexpr bool is_same_size_v = is_same_size<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_SAME_SIZE_H
