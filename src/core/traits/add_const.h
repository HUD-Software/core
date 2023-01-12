#pragma once
#ifndef HD_INC_CORE_TRAITS_ADD_CONST_H
#define HD_INC_CORE_TRAITS_ADD_CONST_H

namespace hud
{

    /** Add const qualifier to the type type_t. */
    template <typename type_t>
    struct add_const
    {
        using type = type_t const;
    };

    /** Equivalent of typename add_const<type_t>::type. */
    template <typename type_t>
    using add_const_t = typename add_const<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_ADD_CONST_H