#pragma once
#ifndef HD_INC_OSLAYER_TUPLE_ELEMENT_H
#define HD_INC_OSLAYER_TUPLE_ELEMENT_H
#include "../Traits/AddConst.h"
#include "../Traits/AddVolatile.h"
#include "../Traits/AddCV.h"

namespace hud {

    /**
    * Class template designed to access the type of an elements in a tuple-like type at a given index.
    * The class is itself undefined for the generic types, but a specialization for tuple-like types ( Tuple, Pair, hdFixedArray )
    * in their respective headers, also having a Type member defined to the appropriate type.
    * For const and/or volatile-qualified tuple-like objects, the class is specialized so that its Type member is itself non cv-qualified.
    * Provides the member constant Type which the type of the elements in a tuple-like type at a given index.
    * @tparam index The index to get
    * @tparam TupleLike The tuple-like type
    */
    template<const usize index, typename TupleType>
    struct TupleElement;

    /**
    * Specialization for const qualified tuple-like objects.
    * @tparam index The index to get
    * @tparam TupleLike The tuple-like type
    */
    template<const usize index, typename TupleType>
    struct TupleElement<index, const TupleType>
        : AddConst<typename TupleElement<index, TupleType>::Type> {
    };

    /**
    * Specialization for volatile qualified tuple-like objects.
    * @tparam index The index to get
    * @tparam TupleLike The tuple-like type
    */
    template<const usize index, typename TupleType>
    struct TupleElement<index, volatile TupleType> 
        : AddVolatile<typename TupleElement<index, TupleType>::Type> {
    };

    /**
    * Specialization for const volatile qualified tuple-like objects.
    * @tparam at The index to get
    * @tparam TupleLike The tuple-like type
    */
    template<const usize index, typename TupleType>
    struct TupleElement<index, const volatile TupleType> 
        : AddCV<typename TupleElement<index, TupleType>::Type> {
    };

    /** Equalivent of typename TupleElement<at, TupleType>::Type. */
    template<const usize index, typename TupleType>
    using TupleElementT = typename TupleElement<index, TupleType>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TUPLE_ELEMENT_H