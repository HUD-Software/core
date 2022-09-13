#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_REMOVE_CONST_H
#define HD_INC_OSLAYER_TRAITS_REMOVE_CONST_H

namespace hud {

    /** Remove const qualifier of a type if type is const qualified, else do nothing. */
    template<typename T>
    struct RemoveConst {
        using Type = T;
    };
    template<typename T>
    struct RemoveConst<const T> {
        using Type = T;
    };

    /** Equivalent of typename RemoveConst<T>::Type. */
    template<typename T>
    using RemoveConstT = typename RemoveConst<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_REMOVE_CONST_H