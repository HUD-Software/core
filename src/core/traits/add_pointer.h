#pragma once
#ifndef HD_INC_CORE_TRAITS_ADD_POINTER_H
#define HD_INC_CORE_TRAITS_ADD_POINTER_H
#include "void_t.h"
#include "remove_reference.h"

namespace hud {

    /** Provides a member typedef type which is remove_reference_t<type_t>* if type_t is a reference type. Otherwise type is type_t*. */
    template<typename type_t, typename = void>
    struct add_pointer {
        using type = type_t;
    };
    template<typename type_t>
    struct add_pointer<type_t, void_t<remove_reference_t<type_t>*>> {
        using type = remove_reference_t<type_t>*;
    };

    /** Equivalent of typename add_pointer<type_t>::type. */
    template <typename type_t>
    using add_pointer_t = typename add_pointer<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_ADD_POINTER_H
