#ifndef HD_INC_OSLAYER_traits_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_traits_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#include "add_lvalue_reference.h"
#include "add_const.h"
#include "is_explicitly_constructible.h"

namespace hud {

    /** Checks whether T is an explicitly copy constructible type with U. */
    template<typename T, typename U = T>
    struct IsExplicitlyCopyConstructible
        : IsExplicitlyConstructible< T, AddLValueReferenceT<AddConstT<U>> > {
    };

    /** Equivalent of IsExplicitlyCopyConstructible<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsExplicitlyCopyConstructibleV = IsExplicitlyCopyConstructible<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H