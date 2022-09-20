#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_MEMBER_POINTER_H
#define HD_INC_CORE_TRAITS_IS_MEMBER_POINTER_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether type_t is a pointer to non-static member object or a pointer to non-static member function. */
    template<typename type_t>
    struct is_member_pointer
        : hud::false_type {
    };
    template<typename type_t, typename class_t>
    struct is_member_pointer<type_t class_t::*>
        : hud::true_type {
    };

    /** Equivalent of is_member_pointer<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_member_pointer_v = is_member_pointer<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_MEMBER_POINTER_H