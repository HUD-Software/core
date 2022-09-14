#pragma once
#ifndef HD_INC_OSLAYER_traits_REMOVE_CVREF_H
#define HD_INC_OSLAYER_traits_REMOVE_CVREF_H
#include "remove_cv.h"
#include "remove_reference.h"

namespace hud {

    /** Remove the reference and the topmost const and volatile qualifier of a type */
    template<typename T>
    struct RemoveCVRef 
        : RemoveCV<RemoveReferenceT<T>> {
    };

    /** Equivalent of typename RemoveCVRef<T>::Type. */
    template<typename T>
    using RemoveCVRefT = typename RemoveCVRef<T>::Type;
}

#endif // HD_INC_OSLAYER_traits_REMOVE_CVREF_H