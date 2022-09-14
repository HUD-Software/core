#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_OBJECT_H
#define HD_INC_OSLAYER_traits_IS_OBJECT_H
#include "not.h"
#include "or.h"
#include "is_function.h"
#include "is_reference.h"
#include "is_void.h"

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

#endif // HD_INC_OSLAYER_traits_IS_OBJECT_H
