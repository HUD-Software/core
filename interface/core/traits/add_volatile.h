#ifndef HD_INC_CORE_TRAITS_ADD_VOLATILE_H
#define HD_INC_CORE_TRAITS_ADD_VOLATILE_H

namespace hud
{

    /** Add volatile qualifier to the type type_t. */
    template<typename type_t>
    struct add_volatile
    {
        using type = type_t volatile;
    };

    /** Equivalent of typename add_volatile<type_t>::type. */
    template<typename type_t>
    using add_volatile_t = typename add_volatile<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_ADD_VOLATILE_H