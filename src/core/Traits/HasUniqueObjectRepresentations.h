#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATION_H
#define HD_INC_OSLAYER_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATION_H
#include "IntegralConstant.h"
#include "RemoveCV.h"
#include "RemoveAllExtents.h"

namespace hud {

    /**
    * Check if T is TriviallyCopyable and if any two objects of type T with the same value have the same object representation,
    * For the purpose of this trait, two arrays have the same value if their elements have the same values,
    * two non-union classes have the same value if their direct subobjects have the same value,
    * and two unions have the same value if they have the same active member and the value of that member are the same.
    * It is implementation-defined which scalar types satisfy this trait,
    * but unsigned (until C++20) integer types that do not use padding bits are guaranteed to have unique object representations.
    * The behavior is undefined if T is an incomplete type other than (possibly cv-qualified) void or array of unknown bound.
    */
    template<typename T>
    struct HasUniqueObjectRepresentations
        : BoolConstant< __has_unique_object_representations(RemoveCVT<RemoveAllExtentsT<T>>)> {
    };

    /** Equivalent of HasUniqueObjectRepresentations<T>::Value. */
    template<typename T>
    inline constexpr bool HasUniqueObjectRepresentationsV = HasUniqueObjectRepresentations<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATION_H
