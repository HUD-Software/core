#ifndef HD_INC_CORE_TRAITS_UNDERLYING_TYPE_H
#define HD_INC_CORE_TRAITS_UNDERLYING_TYPE_H

namespace hud
{

    /*
     * Retrieves the underlying type of enum type type_t without const volatile qualifiers.
     * The underlying type of an enum declared with enum class is int unless a different type is specified on declaration.
     */
    template<typename type_t>
    struct underlying_type
    {
        using type = __underlying_type(type_t);
    };

    /* Equivalent of typename underlying_type<type_t>::type. */
    template<typename type_t>
    using underlying_type_t = typename underlying_type<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_UNDERLYING_TYPE_H