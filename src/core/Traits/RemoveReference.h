#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_REMOVE_REFERENCE_H
#define HD_INC_OSLAYER_TRAITS_REMOVE_REFERENCE_H

namespace hud {

    /**
    * Remove the reference of a type.
    * If the type T is a reference type, provides the member typedef Type which is the type referred to by T. Otherwise type is T.
    */
    template<typename T>
    struct RemoveReference {
        using Type = T;
    };

    template<typename T>
    struct RemoveReference<T&> {
        using Type = T;
    };
    template<typename T>
    struct RemoveReference<T&&> {
        using Type = T;
    };

    /** Equivalent of typename RemoveReference<T>::Type. */
    template <typename T>
    using RemoveReferenceT = typename RemoveReference<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_REMOVE_REFERENCE_H