#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_REMOVE_CV_H
#define HD_INC_OSLAYER_TRAITS_REMOVE_CV_H
#include "RemoveVolatile.h"
#include "RemoveConst.h"

namespace hud {

    /** 
    * Remove const and volatile qualifier of a type.
    * Note: Removing const/volatile from 'const volatile void*' does not modify the type, because the pointer itself is neither const nor volatile.
    */
    template<typename T>
    struct RemoveCV 
        : RemoveVolatile<RemoveConstT<T>> {
    };

    /** Equivalent of typename RemoveCV<T>::Type */
    template<typename T>
    using RemoveCVT = typename RemoveCV<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_REMOVE_CV_H