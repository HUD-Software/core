#pragma once
#ifndef HD_INC_CORE_TRAITS_REMOVE_VOLATILE_H
#define HD_INC_CORE_TRAITS_REMOVE_VOLATILE_H

namespace hud
{

    /** Remove volatile qualifier of a type.  */
    template <typename type_t>
    struct remove_volatile
    {
        using type = type_t;
    };

    template <typename type_t>
    struct remove_volatile<volatile type_t>
    {
        using type = type_t;
    };

    /** Equivalent of typename remove_volatile<type_t>::type. */
    template <typename type_t>
    using remove_volatile_t = typename remove_volatile<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_VOLATILE_H
