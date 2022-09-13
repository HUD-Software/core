#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_REFERENCEABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_REFERENCEABLE_H
#include "IntegralConstant.h"
#include "VoidT.h"

namespace hud {

    /** Check if a type is referenceable (T& is well-formed). */
    template<typename T, typename = void>
    struct IsReferenceable
        : FalseType {
    };

    template<typename T>
    struct IsReferenceable<T, VoidT<T&> >
        : TrueType {
    };

    /** Equivalent of IsReferenceable<T>::Value. */
    template<typename T>
    inline constexpr bool IsReferenceableV = IsReferenceable<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_REFERENCEABLE_H