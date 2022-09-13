#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_EXTENT_H
#define HD_INC_OSLAYER_TRAITS_EXTENT_H
#include "IntegralConstant.h"

namespace hud {

    /**
    * Retrieves the extent (number of elements) of the dimension_index of type T.
    * If T is an array that has a rank greater than dimension_index, the extent is the bound (i.e., the number of elements) of the dimension_index.
    * In all other cases, and in the case that dimension_index is zero and T is an array of unknown bound, the extent value is zero.
    */
    template<typename T, usize dimension_index = 0>
    struct Extent
        : IntegralConstant<usize,0> {
    };
    template<typename T, usize extent>
    struct Extent<T[extent], 0>
        : IntegralConstant<usize, extent> {
    };
    template<typename T, usize dimension_index, usize extent>
    struct Extent<T[extent], dimension_index>
        : Extent<T, dimension_index - 1> {
    };
    template<typename T, usize dimension_index>
    struct Extent<T[], dimension_index>
        : Extent<T, dimension_index - 1> {
    };

    /** Equivalent of Extent<T, dimension_index>::Value. */
    template<typename T, usize dimension_index = 0>
    inline constexpr usize ExtentV = Extent<T, dimension_index>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_EXTENT_H