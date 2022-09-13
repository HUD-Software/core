#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_ADD_RVALUE_REFERENCE_H
#define HD_INC_OSLAYER_TRAITS_ADD_RVALUE_REFERENCE_H
#include "IsReferenceable.h"

namespace hud {

    /** Provides a member typedef Type which is T&& if T is referenceable. Otherwise type is T. */
    template<typename T, bool = IsReferenceableV<T>>
    struct AddRValueReference {
        using Type = T;
    };
    template<typename T>
    struct AddRValueReference<T, true> {
        using Type = T&&;
    };

    /** Equivalent of typename AddLValueReference<T>::Type. */
    template <typename T>
    using AddRValueReferenceT = typename AddRValueReference<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_ADD_RVALUE_REFERENCE_H
