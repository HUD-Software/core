#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_MEMBER_FUNCTION_POINTER_H
#define HD_INC_OSLAYER_TRAITS_IS_MEMBER_FUNCTION_POINTER_H
#include "IntegralConstant.h"
#include "IsFunction.h"

namespace hud {

    /**  Checks whether T is a non-static member function pointer. */
    template <typename T>
    struct IsMemberFunctionPointer
        : FalseType {
    };
    template<typename T, typename Class>
    struct IsMemberFunctionPointer<T Class::*>
        : IsFunction<T> {
    };

    /** Equivalent of IsMemberFunctionPointer<T>::Value. */
    template<typename T>
    inline constexpr bool IsMemberFunctionPointerV = IsMemberFunctionPointer<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_MEMBER_FUNCTION_POINTER_H