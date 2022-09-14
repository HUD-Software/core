#pragma once
#ifndef HD_INC_OSLAYER_traits_ADD_LVALUE_REFERENCE_H
#define HD_INC_OSLAYER_traits_ADD_LVALUE_REFERENCE_H
#include "is_referenceable.h"

namespace hud {

    /** Provides a member typedef Type which is T& if T is referenceable. Otherwise type is T. */
    template<typename T, bool = IsReferenceableV<T>>
    struct AddLValueReference {
        using Type = T;
    };
    template<typename T>
    struct AddLValueReference<T,true> {
        using Type = T&;
    };

    /** Equivalent of typename AddLValueReference<T>::Type. */
    template <typename T>
    using AddLValueReferenceT = typename AddLValueReference<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_ADD_LVALUE_REFERENCE_H
