#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_ADD_CONST_H
#define HD_INC_OSLAYER_TRAITS_ADD_CONST_H

namespace hud {

    /** Add const qualifier to the type T. */
    template<typename T>
    struct AddConst {
        using Type = T const;
    };

    /** Equivalent of typename AddConst<T>::Type. */
    template<typename T>
    using AddConstT = typename AddConst<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_ADD_CONST_H