#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_BITWISE_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_BITWISE_COPY_CONSTRUCTIBLE_H
#include "disjunction.h"
#include "conjunction.h"
#include "is_pointer.h"
#include "is_member_pointer.h"
#include "is_same_size.h"
#include "is_integral.h"
#include "is_enum.h"
#include "is_same.h"
#include "is_trivially_copy_constructible.h"

namespace hud {

    /** Checks whether type_t is a bitwise copy constructible type ( Constructible with a bitwise memory copy ) with the argument type u_type_t. */
    template<typename type_t, typename u_type_t>
    struct is_bitwise_copy_constructible
        : hud::disjunction<
        hud::conjunction<  // If both are pointers or pointer's to member it's bitwise comparable
        hud::disjunction<is_pointer<type_t>, is_member_pointer<type_t>>,
        hud::disjunction<is_pointer<u_type_t>, is_member_pointer<u_type_t>>
        >,
        hud::conjunction< // or if same size and both are integral or enums it's bitwise comparable
        hud::is_same_size<type_t, u_type_t>,
        hud::disjunction<
        hud::conjunction<is_integral<type_t>, is_integral<u_type_t>>, // Assuming two's-complement
        hud::conjunction<is_enum<type_t>, is_enum<u_type_t>>
        >
        >,
        hud::conjunction< // or if same type trivially copy constructible
        hud::is_same<type_t, u_type_t>,
        is_trivially_copy_constructible<type_t, u_type_t>
        >
        > {
    };


    /** Equivalent of is_bitwise_copy_constructible<type_t, u_type_t = type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    inline constexpr bool is_bitwise_copy_constructible_v = is_bitwise_copy_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BITWISE_COPY_CONSTRUCTIBLE_H