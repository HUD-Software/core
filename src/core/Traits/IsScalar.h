#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_SCALAR_H
#define HD_INC_OSLAYER_TRAITS_IS_SCALAR_H
#include "IntegralConstant.h"
#include "Or.h"
#include "IsArithmetic.h"
#include "IsEnum.h"
#include "IsPointer.h"
#include "IsMemberPointer.h"
#include "IsNullptr.h"


namespace hud {

    /**
    * Checks whether T is a scalar type including cv-qualified variants.
    * A scalar type is a type that has built-in functionality for the addition operator without overloads (arithmetic, pointer, member pointer, enum and hud::ptr::null).
    */
    template<typename T>
    struct IsScalar
        : Or< IsArithmetic<T>, IsEnum<T>, IsPointer<T>, IsMemberPointer<T>, IsNullptr<T> > {
    };

    /** Equivalent of IsScalar<T>::Value. */
    template<typename T>
    inline constexpr bool IsScalarV = IsScalar<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_SCALAR_H
