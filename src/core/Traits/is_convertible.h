#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#define HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#include "integral_constant.h"

namespace hud {

    /** Checks whether from_t is implicitly convertible to to_t. */
    template <typename from_t, typename to_t>
    struct is_convertible
        : hud::bool_constant<__is_convertible_to(from_t, to_t)> {}
    ;

    /** Equivalent of hud::is_convertible<from_t,to_t>::value. */
    template <typename from_t, typename to_t>
    inline constexpr bool is_convertible_v = hud::is_convertible<from_t, to_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H