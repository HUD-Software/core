#pragma once
#ifndef HD_INC_CORE_TRAITS_REMOVE_ALL_EXTENTS_H
#define HD_INC_CORE_TRAITS_REMOVE_ALL_EXTENTS_H

namespace hud {

    /** 
    * Retrieves the type of elements in the deepest dimension of type_t if type_t is an array type.
    * If type_t is an array type, this is the same type as the elements in its deepest dimension. Otherwise, member type is the same as type_t.
    * Notice that, for multidimensional arrays, all dimensions are removed (see remove_extent to remove a single dimension from an array type).
    */
    template<typename type_t>
    struct remove_all_extents {
        using type = type_t;
    };

    template<typename type_t>
    struct remove_all_extents<type_t[]>
        : remove_all_extents<type_t> {
    };

    template<typename type_t, usize extent>
    struct remove_all_extents<type_t[extent]>
        : remove_all_extents<type_t> {
    };

    /** Equivalent of typename remove_all_extents<type_t>::type. */
    template <typename type_t>
    using remove_all_extents_t = typename remove_all_extents<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_ALL_EXTENTS_H
