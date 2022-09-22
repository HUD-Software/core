#pragma once
#ifndef HD_INC_CORE_TEMPLATES_SELECT_DELETER_POINTER_POINTER_TYPE_H
#define HD_INC_CORE_TEMPLATES_SELECT_DELETER_POINTER_POINTER_TYPE_H
#include "../traits/remove_reference.h"
#include "../traits/void_t.h"

namespace hud {

    /**
    * Retrieves the type of the pointer depending of the deleter_t.
    * Provides the typedef pointer_type equals to deleter_type::pointer_type if the deleter has pointer_type member. Otherwise, the typedef pointer_type is equal to type_t*.
    * @tparam type_t The pointer type of the unique pointer
    * @tparam deleter_t The deleter type of the unique pointer
    */
    template <typename type_t, typename deleter_t, typename = void>
    struct select_deleter_pointer_type {
        using pointer_type = type_t*;
    };
    template <typename type_t, typename deleter_t>
    struct select_deleter_pointer_type<type_t, deleter_t, void_t<typename hud::remove_reference_t<deleter_t>::pointer_type>> {
        using pointer_type = typename hud::remove_reference_t<deleter_t>::pointer_type;
    };

    /** Helper alias template equivalent to select_deleter_pointer_type<type_t, deleter_t>::pointer_type. */
    template<typename type_t, typename deleter_t>
    using select_deleter_pointer_type_t = typename select_deleter_pointer_type<type_t, deleter_t>::pointer_type;

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_SELECT_DELETER_POINTER_POINTER_TYPE_H