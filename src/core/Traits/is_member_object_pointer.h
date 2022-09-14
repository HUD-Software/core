#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_MEMBER_OBJECT_POINTER_H
#define HD_INC_CORE_TRAITS_IS_MEMBER_OBJECT_POINTER_H
#include "integral_constant.h"
#include "conjunction.h"
#include "negation.h"
#include "is_member_function_pointer.h"
#include "is_member_pointer.h"

namespace hud {

    /** Checks whether type_t is a non-static member object. */
    template<typename type_t>
    struct is_member_object_pointer
        : conjunction<is_member_pointer<type_t>, negation<is_member_function_pointer<type_t>>> {
    };

    /** Equivalent of is_member_object_pointer<type_t>::value */
    template<typename type_t>
    inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_MEMBER_OBJECT_POINTER_H