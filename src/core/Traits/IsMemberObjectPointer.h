#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_MEMBER_OBJECT_POINTER_H
#define HD_INC_OSLAYER_TRAITS_IS_MEMBER_OBJECT_POINTER_H
#include "IntegralConstant.h"
#include "And.h"
#include "Not.h"
#include "IsMemberFunctionPointer.h"
#include "IsMemberPointer.h"

namespace hud {

    /** Checks whether T is a non-static member object. */
    template<typename T>
    struct IsMemberObjectPointer
        : And<IsMemberPointer<T>, Not<IsMemberFunctionPointer<T>>> {
    };

    /** Equivalent of IsMemberObjectPointer<T>::Value */
    template<typename T>
    inline constexpr bool IsMemberObjectPointerV = IsMemberObjectPointer<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_MEMBER_OBJECT_POINTER_H