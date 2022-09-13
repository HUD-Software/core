#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_REMOVE_EXTENT_H
#define HD_INC_OSLAYER_TRAITS_REMOVE_EXTENT_H

namespace hud {

    /**
    * Remove extent ([]) of an bounded or unbounded array.
    * If T is an array type, this is the same type as its elements. Otherwise, member type is the same as T.
    * Notice that, for multidimensional arrays, only the first array dimension is removed(see RemoveAllExtents to obtain the type of the elements in the deepest dimension).
    */
    template<typename T>
    struct RemoveExtent {
        using Type = T;
    };

    template<typename T>
    struct RemoveExtent<T[]> {
        using Type = T;
    };

    template<typename T, size_t size>
    struct RemoveExtent<T[size]> {
        using Type = T;
    };

    /** Equivalent of typename RemoveExtent<T>::Type. */
    template<typename T>
    using RemoveExtentT = typename RemoveExtent<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_REMOVE_EXTENT_H