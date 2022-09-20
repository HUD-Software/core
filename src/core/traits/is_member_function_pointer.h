#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_MEMBER_FUNCTION_POINTER_H
#define HD_INC_CORE_TRAITS_IS_MEMBER_FUNCTION_POINTER_H
#include "integral_constant.h"
#include "is_function.h"

namespace hud {

    /**  Checks whether type_t is a non-static member function pointer. */
    template <typename type_t>
    struct is_member_function_pointer
        : hud::false_type {
    };
    template<typename type_t, typename class_t>
    struct is_member_function_pointer<type_t class_t::*>
        : is_function<type_t> {
    };

    /** Equivalent of is_member_function_pointer<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_MEMBER_FUNCTION_POINTER_H