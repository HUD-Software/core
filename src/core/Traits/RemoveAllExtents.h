#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_REMOVE_ALL_EXTENTS_H
#define HD_INC_OSLAYER_TRAITS_REMOVE_ALL_EXTENTS_H

namespace hud {

    /** 
    * Retrieves the type of elements in the deepest dimension of T if T is an array type.
    * If T is an array type, this is the same type as the elements in its deepest dimension. Otherwise, member type is the same as T.
    * Notice that, for multidimensional arrays, all dimensions are removed (see RemoveExtent to remove a single dimension from an array type).
    */
    template<typename T>
    struct RemoveAllExtents {
        using Type = T;
    };

    template<typename T>
    struct RemoveAllExtents<T[]>
        : RemoveAllExtents<T> {
    };

    template<typename T, usize size>
    struct RemoveAllExtents<T[size]>
        : RemoveAllExtents<T> {
    };

    /** Equivalent of typename RemoveAllExtents<T>::Type. */
    template <typename T>
    using RemoveAllExtentsT = typename RemoveAllExtents<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_REMOVE_ALL_EXTENTS_H
