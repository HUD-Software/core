#ifndef HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H
#include "AddLValueReference.h"
#include "AddConst.h"
#include "IsExplicitlyConstructible.h"

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

#endif // HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_COPY_CONSTRUCTIBLE_H