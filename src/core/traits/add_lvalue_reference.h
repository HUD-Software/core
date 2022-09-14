#pragma once
#ifndef HD_INC_CORE_TRAITS_ADD_LVALUE_REFERENCE_H
#define HD_INC_CORE_TRAITS_ADD_LVALUE_REFERENCE_H
#include "is_referenceable.h"

namespace hud {

    /** Provides a member typedef Type which is type_t& if type_t is referenceable. Otherwise type is type_t. */
    template<typename type_t, bool = is_referenceable_v<type_t>>
    struct add_lvalue_reference {
        using type = type_t;
    };
    template<typename type_t>
    struct add_lvalue_reference<type_t,true> {
        using type = type_t&;
    };

    /** Equivalent of typename add_lvalue_reference<type_t>::type. */
    template <typename type_t>
    using add_lvalue_reference_t = typename add_lvalue_reference<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_ADD_LVALUE_REFERENCE_H
