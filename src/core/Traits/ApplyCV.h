#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_APPLY_CONST_VOLATILE_H
#define HD_INC_OSLAYER_TRAITS_APPLY_CONST_VOLATILE_H

namespace hud {

    /* Conditionaly add const volatile qualifier to the type T. */
    template<typename T, bool apply_const, bool apply_volatile >
    struct ApplyCV {
        // Do not apply const and volatile
        using Type = T;
    };

    template<typename T>
    struct ApplyCV<T, true, true> {
        // Apply const and volatile
        using Type = T const volatile;
    };

    template<typename T>
    struct ApplyCV<T, true, false> {
        // Apply const only
        using Type = T const;
    };

    template<typename T>
    struct ApplyCV<T, false, true> {
        // Apply volatile only
        using Type = T volatile;
    };

    /** Equivalent of typename ApplyCV<T, apply_const, apply_volatile>::Type. */
    template<typename T, bool apply_const, bool apply_volatile >
    using ApplyCVT = typename ApplyCV<T, apply_const, apply_volatile>::Type;
}

#endif // HD_INC_OSLAYER_TRAITS_APPLY_CONST_VOLATILE_H