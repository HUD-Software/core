#pragma once
#ifndef HD_INC_CORE_TRAITS_APPLY_CONST_VOLATILE_H
#define HD_INC_CORE_TRAITS_APPLY_CONST_VOLATILE_H

namespace hud
{

    /* Conditionaly add const volatile qualifier to the type type_t. */
    template <typename type_t, bool apply_const, bool apply_volatile>
    struct apply_cv
    {
        // Do not apply const and volatile
        using type = type_t;
    };

    template <typename type_t>
    struct apply_cv<type_t, true, true>
    {
        // Apply const and volatile
        using type = type_t const volatile;
    };

    template <typename type_t>
    struct apply_cv<type_t, true, false>
    {
        // Apply const only
        using type = type_t const;
    };

    template <typename type_t>
    struct apply_cv<type_t, false, true>
    {
        // Apply volatile only
        using type = type_t volatile;
    };

    /** Equivalent of typename apply_cv<type_t, apply_const, apply_volatile>::type. */
    template <typename type_t, bool apply_const, bool apply_volatile>
    using apply_cv_t = typename apply_cv<type_t, apply_const, apply_volatile>::type;
}

#endif // HD_INC_CORE_TRAITS_APPLY_CONST_VOLATILE_H