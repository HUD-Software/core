#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_MEMBER_POINTER_H
#define HD_INC_OSLAYER_TRAITS_IS_MEMBER_POINTER_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a pointer to non-static member object or a pointer to non-static member function. */
    template<typename T>
    struct IsMemberPointer
        : FalseType {
    };
    template<typename T, typename Class>
    struct IsMemberPointer<T Class::*>
        : TrueType {
    };

    /** Equivalent of IsMemberPointer<T>::Value. */
    template<typename T>
    inline constexpr bool IsMemberPointerV = IsMemberPointer<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_MEMBER_POINTER_H