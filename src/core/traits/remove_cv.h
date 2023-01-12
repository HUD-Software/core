#pragma once
#ifndef HD_INC_CORE_TRAITS_REMOVE_CV_H
#define HD_INC_CORE_TRAITS_REMOVE_CV_H
#include "remove_volatile.h"
#include "remove_const.h"

namespace hud
{

    /**
     * Remove const and volatile qualifier of a type.
     * Note: Removing const/volatile from 'const volatile void*' does not modify the type, because the pointer itself is neither const nor volatile.
     */
    template <typename type_t>
    struct remove_cv
        : remove_volatile<remove_const_t<type_t>>
    {
    };

    /** Equivalent of typename remove_cv<type_t>::type */
    template <typename type_t>
    using remove_cv_t = typename remove_cv<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_CV_H