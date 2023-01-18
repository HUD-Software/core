#ifndef HD_INC_CORE_TRAITS_ADD_CONST_VOLATILE_H
#define HD_INC_CORE_TRAITS_ADD_CONST_VOLATILE_H

namespace hud
{

    /** Add const volatile qualifier to the type type_t. */
    template<typename type_t>
    struct add_cv
    {
        using type = type_t const volatile;
    };

    /** Equivalent of typename add_cv<type_t>::type. */
    template<typename type_t>
    using add_cv_t = typename add_cv<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_ADD_CONST_VOLATILE_H