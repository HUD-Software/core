#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_REMOVE_POINTER_H
#define HD_INC_OSLAYER_TRAITS_REMOVE_POINTER_H

namespace hud {

    /**
    * Remove pointer of a type, including const and volatile qualifiers.
    * If the type T is a reference type, provides the member typedef Type which is the type referred to by T. Otherwise type is T.
    */
    template<typename T>
    struct RemovePointer {
        using Type = T;
    };

    template<typename T>
    struct RemovePointer<T*> {
        using Type = T;
    };

    template<typename T>
    struct RemovePointer<T* const> {
        using Type = T;
    };

    template<typename T>
    struct RemovePointer<T* volatile> {
        using Type = T;
    };

    template<typename T>
    struct RemovePointer<T* const volatile> {
        using Type = T;
    };

    /** Equivalent of typename RemoveReference<T>::Type. */
    template <typename T>
    using RemovePointerT = typename RemovePointer<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_REMOVE_POINTER_H