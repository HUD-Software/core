#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_SCALAR_H
#define HD_INC_CORE_TRAITS_IS_SCALAR_H
#include "integral_constant.h"
#include "disjunction.h"
#include "is_arithmetic.h"
#include "is_enum.h"
#include "is_pointer.h"
#include "is_member_pointer.h"
#include "is_nullptr.h"


namespace hud {

    /**
    * Checks whether type_t is a scalar type including cv-qualified variants.
    * A scalar type is a type that has built-in functionality for the addition operator without overloads (arithmetic, pointer, member pointer, enum and hud::ptr::null).
    */
    template<typename type_t>
    struct is_scalar
        : hud::disjunction< is_arithmetic<type_t>, is_enum<type_t>, is_pointer<type_t>, is_member_pointer<type_t>, is_nullptr<type_t> > {
    };

    /** Equivalent of is_scalar<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_scalar_v = is_scalar<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_SCALAR_H
