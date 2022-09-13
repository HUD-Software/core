#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_REMOVE_VOLATILE_H
#define HD_INC_OSLAYER_TRAITS_REMOVE_VOLATILE_H

namespace hud {

    /** Remove volatile qualifier of a type.  */
    template<typename T>
    struct RemoveVolatile {
        using Type = T;
    };

    template<typename T>
    struct RemoveVolatile<volatile T> {
        using Type = T;
    };

    /** Equivalent of typename RemoveVolatile<T>::Type. */
    template<typename T>
    using RemoveVolatileT = typename RemoveVolatile<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_REMOVE_VOLATILE_H
