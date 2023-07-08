#ifndef HD_INC_CORE_TRAITS_EXTENT_H
#define HD_INC_CORE_TRAITS_EXTENT_H
#include "integral_constant.h"

namespace hud
{

    /**
     * Retrieves the extent (number of elements) of the dimension_index of type type_t.
     * If type_t is an array that has a rank greater than dimension_index, the extent is the bound (i.e., the number of elements) of the dimension_index.
     * In all other cases, and in the case that dimension_index is zero and type_t is an array of unknown bound, the extent value is zero.
     */
    template<typename type_t, usize dimension_index = 0>
    struct extent
        : hud::integral_constant<usize, 0>
    {
    };

    template<typename type_t, usize extent_size>
    struct extent<type_t[extent_size], 0>
        : hud::integral_constant<usize, extent_size>
    {
    };

    template<typename type_t, usize dimension_index, usize extent_size>
    struct extent<type_t[extent_size], dimension_index>
        : hud::extent<type_t, dimension_index - 1>
    {
    };

    template<typename type_t, usize dimension_index>
    struct extent<type_t[], dimension_index>
        : hud::extent<type_t, dimension_index - 1>
    {
    };

    /** Equivalent of extent<type_t, dimension_index>::value. */
    template<typename type_t, usize dimension_index = 0>
    inline constexpr usize extent_v = hud::extent<type_t, dimension_index>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_EXTENT_H