#pragma once
#ifndef HD_INC_CORE_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATION_H
#define HD_INC_CORE_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATION_H
#include "integral_constant.h"
#include "remove_cv.h"
#include "remove_all_extents.h"

namespace hud {

    /**
    * Check if type_t has unique object representations.
    * If is TriviallyCopyable and if any two objects of type type_t with the same value have the same object representation,
    * For the purpose of this trait, two arrays have the same value if their elements have the same values,
    * two non-union classes have the same value if their direct subobjects have the same value,
    * and two unions have the same value if they have the same active member and the value of that member are the same.
    * It is implementation-defined which scalar types satisfy this trait,
    * but unsigned (until C++20) integer types that do not use padding bits are guaranteed to have unique object representations.
    * The behavior is undefined if type_t is an incomplete type other than (possibly cv-qualified) void or array of unknown bound.
    */
    template<typename type_t>
    struct has_unique_object_representations
        : hud::bool_constant< __has_unique_object_representations(remove_cv_t<hud::remove_all_extents_t<type_t>>)> {
    };

    /** Equivalent of has_unique_object_representations<type_t>::value. */
    template<typename type_t>
    inline constexpr bool has_unique_object_representations_v = has_unique_object_representations<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATION_H
