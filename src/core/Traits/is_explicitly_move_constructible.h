#ifndef HD_INC_OSLAYER_traits_IS_EXPLICITLY_MOVE_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_traits_IS_EXPLICITLY_MOVE_CONSTRUCTIBLE_H
#include "add_rvalue_reference.h"
#include "is_explicitly_constructible.h"

namespace hud {

    /** Checks whether T is an explicitly move constructible type with U.*/
    template<typename T, typename U = T>
    struct IsExplicitlyMoveConstructible
        : IsExplicitlyConstructible < T, AddRValueReferenceT<U> > {
    };

    /**Equivalent of IsExplicitlyMoveConstructible<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsExplicitlyMoveConstructibleV = IsExplicitlyMoveConstructible<T, U>::Value;
}

#endif // HD_INC_OSLAYER_traits_IS_EXPLICITLY_MOVE_CONSTRUCTIBLE_H