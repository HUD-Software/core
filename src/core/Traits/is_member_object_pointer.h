#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_MEMBER_OBJECT_POINTER_H
#define HD_INC_OSLAYER_traits_IS_MEMBER_OBJECT_POINTER_H
#include "integral_constant.h"
#include "and.h"
#include "not.h"
#include "is_member_function_pointer.h"
#include "is_member_pointer.h"

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

#endif // HD_INC_OSLAYER_traits_IS_MEMBER_OBJECT_POINTER_H