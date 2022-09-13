#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_ADD_CONST_VOLATILE_H
#define HD_INC_OSLAYER_TRAITS_ADD_CONST_VOLATILE_H

namespace hud {

    /** Add const volatile qualifier to the type T. */
    template<typename T>
    struct AddCV {
        using Type = T const volatile;
    };

    /** Equivalent of typename AddCV<T>::Type. */
    template<typename T>
    using AddCVT = typename AddCV<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_ADD_CONST_VOLATILE_H