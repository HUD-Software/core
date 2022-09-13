#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_ADD_VOLATILE_H
#define HD_INC_OSLAYER_TRAITS_ADD_VOLATILE_H

namespace hud {

    /** Add volatile qualifier to the type T. */
    template<typename T>
    struct AddVolatile {
        using Type = T volatile;
    };

    /** Equivalent of typename AddVolatile<T>::Type. */
    template<typename T>
    using AddVolatileT = typename AddVolatile<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_ADD_VOLATILE_H