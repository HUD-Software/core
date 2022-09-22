#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_BITWISE_CONVERTIBLE_H
#define HD_INC_CORE_TRAITS_IS_BITWISE_CONVERTIBLE_H
#include "disjunction.h"
#include "conjunction.h"
#include "is_pointer.h"
#include "is_member_pointer.h"
#include "is_same_size.h"
#include "is_integral.h"
#include "is_enum.h"
#include "is_trivially_copyable.h"

namespace hud {

    /** Checks whether type_t is a bitwise convertible type with the argument type u_type_t. */
    template<typename type_t, typename u_type_t>
    struct is_bitwise_convertible
        : hud::disjunction<
            hud::conjunction< hud::disjunction<is_pointer<type_t>, is_member_pointer<type_t>>, hud::disjunction<is_pointer<u_type_t>, is_member_pointer<u_type_t>>>, // If both are pointers or pointer's to member it's bitwise convertible
            hud::conjunction< hud::disjunction< hud::conjunction<is_integral<type_t>, is_integral<u_type_t>>, hud::conjunction<is_enum<type_t>, is_enum<u_type_t>>>, hud::is_same_size<type_t, u_type_t>> // If same size and both are integral or enums it's bitwise convertible, assuming two's-complement
        > {
    };

    template<typename type_t>
    struct is_bitwise_convertible<type_t, type_t>
        : is_trivially_copyable<type_t> {
    };


    /** Equivalent of is_bitwise_convertible<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t>
    inline constexpr bool is_bitwise_convertible_v = is_bitwise_convertible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BITWISE_CONVERTIBLE_H