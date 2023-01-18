#ifndef HD_INC_CORE_TRAITS_REMOVE_POINTER_H
#define HD_INC_CORE_TRAITS_REMOVE_POINTER_H

namespace hud
{

    /**
     * Remove pointer of a type, including const and volatile qualifiers.
     * If the type type_t is a reference type, provides the member typedef type which is the type referred to by type_t. Otherwise type is type_t.
     */
    template<typename type_t>
    struct remove_pointer
    {
        using type = type_t;
    };

    template<typename type_t>
    struct remove_pointer<type_t *>
    {
        using type = type_t;
    };

    template<typename type_t>
    struct remove_pointer<type_t *const>
    {
        using type = type_t;
    };

    template<typename type_t>
    struct remove_pointer<type_t *volatile>
    {
        using type = type_t;
    };

    template<typename type_t>
    struct remove_pointer<type_t *const volatile>
    {
        using type = type_t;
    };

    /** Equivalent of typename remove_reference<type_t>::type. */
    template<typename type_t>
    using remove_pointer_t = typename remove_pointer<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_POINTER_H