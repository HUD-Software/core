#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_SELECT_DELETER_POINTER_POINTER_TYPE_H
#define HD_INC_OSLAYER_TEMPLATES_SELECT_DELETER_POINTER_POINTER_TYPE_H
#include "../Traits/RemoveReference.h"
#include "../Traits/VoidT.h"

namespace hud {

    /**
    * Retrieves the type of the pointer depending of the Deleter.
    * Provides the typedef PointerType equals to DeleterType::PointerType if the deleter has PointerType member. Otherwise, the typedef PointerType is equal to T*.
    * @tparam T The pointer type of the unique pointer
    * @tparam Deleter The deleter type of the unique pointer
    */
    template <typename T, typename Deleter, typename = void>
    struct SelectDeleterPointerType {
        using PointerType = T*;
    };
    template <typename T, typename Deleter>
    struct SelectDeleterPointerType<T, Deleter, VoidT<typename RemoveReferenceT<Deleter>::PointerType>> {
        using PointerType = typename RemoveReferenceT<Deleter>::PointerType;
    };

    /** Helper alias template equivalent to SelectDeleterPointerType<T, Deleter>::PointerType. */
    template<typename T, typename Deleter>
    using SelectDeleterPointerTypeT = typename SelectDeleterPointerType<T, Deleter>::PointerType;

} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_SELECT_DELETER_POINTER_POINTER_TYPE_H