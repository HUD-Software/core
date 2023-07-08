#ifndef HD_INC_CORE_TRAITS_REMOVE_EXTENT_H
#define HD_INC_CORE_TRAITS_REMOVE_EXTENT_H

namespace hud
{

    /**
     * Remove extent ([]) of an bounded or unbounded array.
     * If type_t is an array type, this is the same type as its elements. Otherwise, member type is the same as type_t.
     * Notice that, for multidimensional arrays, only the first array dimension is removed(see remove_all_extents to obtain the type of the elements in the deepest dimension).
     */
    template<typename type_t>
    struct remove_extent
    {
        using type = type_t;
    };

    template<typename type_t>
    struct remove_extent<type_t[]>
    {
        using type = type_t;
    };

    template<typename type_t, size_t extent>
    struct remove_extent<type_t[extent]>
    {
        using type = type_t;
    };

    /** Equivalent of typename remove_extent<type_t>::type. */
    template<typename type_t>
    using remove_extent_t = typename remove_extent<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_EXTENT_H