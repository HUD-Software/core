#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_OBJECT_H
#define HD_INC_OSLAYER_TRAITS_IS_OBJECT_H
#include "Not.h"
#include "Or.h"
#include "IsFunction.h"
#include "IsReference.h"
#include "IsVoid.h"

namespace hud {

    /** Checks whether T is an object type (that is any possibly cv-qualified type other than function, reference, or void types). */
    template < typename T>
    struct IsObject
        : Not<Or<IsFunction<T>, IsReference<T>, IsVoid<T>>> {
    };

    /** Equivalent of IsObject<T>::Value. */
    template<typename T>
    inline constexpr bool IsObjectV = IsObject<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_OBJECT_H
