#ifndef HD_INC_OSLAYER_traits_IS_IMPLICITLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_traits_IS_IMPLICITLY_COPY_CONSTRUCTIBLE_H
#include "add_lvalue_reference.h"
#include "add_const.h"
#include "is_implicitly_constructible.h"

namespace hud {

    /** Checks whether T is an implicitly copy constructible type with U. */
    template<typename T, typename U = T>
    struct IsImplicitlyCopyConstructible
        : IsImplicitlyConstructible< T, AddLValueReferenceT<AddConstT<U>> > {
    };

    /** Equivalent of IsImplicitlyCopyConstructible<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsImplicitlyCopyConstructibleV = IsImplicitlyCopyConstructible<T, U>::Value;
}

#endif // HD_INC_OSLAYER_traits_IS_IMPLICITLY_COPY_CONSTRUCTIBLE_H