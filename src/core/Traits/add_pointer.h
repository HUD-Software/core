#pragma once
#ifndef HD_INC_OSLAYER_traits_ADD_POINTER_H
#define HD_INC_OSLAYER_traits_ADD_POINTER_H
#include "void_t.h"
#include "remove_reference.h"

namespace hud {

    /** Provides a member typedef Type which is RemoveReferenceT<T>* if T is a reference type. Otherwise Type is T*. */
    template<typename T, typename = void>
    struct AddPointer {
        using Type = T;
    };
    template<typename T>
    struct AddPointer<T, VoidT<RemoveReferenceT<T>*>> {
        using Type = RemoveReferenceT<T>*;
    };

    /** Equivalent of typename AddPointer<T>::Type. */
    template <typename T>
    using AddPointerT = typename AddPointer<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_ADD_POINTER_H
