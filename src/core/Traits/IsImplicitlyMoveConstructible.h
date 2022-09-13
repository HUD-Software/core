#ifndef HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H
#include "AddRValueReference.h"
#include "IsImplicitlyConstructible.h"

namespace hud {

    /** Checks whether T is an implicitly move constructible type with U. */
    template<typename T, typename U = T>
    struct IsImplicitlyMoveConstructible
        : IsImplicitlyConstructible< T, AddRValueReferenceT<U> >{
    };

    /** Equivalent of IsImplicitlyMoveConstructible<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsImplicitlyMoveConstructibleV = IsImplicitlyMoveConstructible<T, U>::Value;
}

#endif // HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_MOVE_CONSTRUCTIBLE_H