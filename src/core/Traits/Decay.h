#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_DECAY_H
#define HD_INC_OSLAYER_TRAITS_DECAY_H
#include "RemoveReference.h"
#include "Conditional.h"
#include "IsArray.h"
#include "RemoveExtent.h"
#include "IsFunction.h"
#include "AddPointer.h"
#include "RemoveCV.h"

namespace hud {

    /**
    * Retrieves the decay type of T.
    * Provides a member typedef Type decay type of T.
    * The decay type of T is the same type that results from the standard conversions that happen when an lvalue expression is used as an rvalue, with its cv-qualifier stripped:
    *   - If T is a function type, a function-to-pointer conversion is applied and the decay type is the same as: AddPointerT<T>
    *   - If T is an array type, an array-to-pointer conversion is applied and the decay type is the same as: AddPointerT<RemoveExtentT<RemoveReferenceT<T>>>
    *   - Otherwise, a regular lvalue-to-rvalue conversion is applied and the decay type is the same as: RemoveCVT<RemoveReferenceT<T>>.
    * This resembles the implicit conversions happening when an argument is passed by value to a function.
    */
    template <class T>
    struct Decay {
    private:
        using NoRefType = RemoveReferenceT<T>;

    public:
        using Type = ConditionalT < IsArrayV<NoRefType>,
                                    RemoveExtentT<NoRefType>*,
                                    ConditionalT<   IsFunctionV<NoRefType>,
                                                    AddPointerT<NoRefType>,
                                                    RemoveCVT<NoRefType>
                                                >
                                  >;
    };

    /** Equivalent of typename Decay<T>::Type. */
    template <typename T>
    using DecayT = typename Decay<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_DECAY_H