#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_COPY_ASSIGNABLE_H
#define HD_INC_OSLAYER_TRAITS_IS_COPY_ASSIGNABLE_H
#include "IsAssignable.h"
#include "AddLValueReference.h"
#include "AddConst.h"

namespace hud {

    /**
    * Checks whether U is a copy assignable type to T.
    * A copy assignable type is a type that can be assigned a value or reference of the same type or another type.
    * This includes scalar types and copy assignable classes.
    * A copy assignable class is a class that has its copy assignment operator defined (either the implicitly provided one or a custom defined one).
    */
    template<typename T, typename U = T>
    struct IsCopyAssignable
        : IsAssignable<AddLValueReferenceT<T>, AddLValueReferenceT<AddConstT<U>>> {
    };

    /** Equivalent of IsCopyAssignable<T, U>::Value. */
    template<typename T, typename U = T>
    inline constexpr bool IsCopyAssignableV = IsCopyAssignable<T, U>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_COPY_ASSIGNABLE_H