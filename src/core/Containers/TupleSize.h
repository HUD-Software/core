#pragma once
#ifndef HD_INC_OSLAYER_TUPLE_SIZE_H
#define HD_INC_OSLAYER_TUPLE_SIZE_H
#include "../Traits/IntegralConstant.h"
#include "../Traits/VoidT.h"

namespace hud {

    /**
    * Class template designed to access the number of elements in a tuple-like type (as a constexpr).
    * The class is itself undefined for the generic types, but a specialization for tuple-like types ( Tuple, Pair, hdFixedArray )
    * in their respective headers, also having a Value member defined to the appropriate constexpr value.
    * For const and/or volatile-qualified tuple-like objects, the class is specialized so that its Value member is itself non cv-qualified.
    * Provides the member constant Value which is equal to the number of elements in the tuple-like type.
    * @tparam TupleLike The tuple-like type
    */
    template<typename TupleLike>
    struct TupleSize;

    namespace details {

        /**
        * Selected when TupleSize<TupleLike>::Value is ill-formed
        * @tparam TupleLike The tuple-like type
        */
        template<typename TupleLike, typename = void>
        struct TupleSizeImpl {
        };

        /**
        * Selected when TupleSize<TupleLike>::Value is well-formed
        * @tparam TupleLike The tuple-like type
        */
        template<typename TupleLike >
        struct TupleSizeImpl< TupleLike, VoidT< decltype(TupleSize<TupleLike>::Value) > >
            : IntegralConstant<usize, TupleSize<TupleLike>::Value> {
        };
    }

    /**
    * Specialization for const qualified tuple-like objects
    * @tparam TupleLike The tuple-like type
    */
    template<typename TupleLike>
    struct TupleSize<const TupleLike> 
        : details::TupleSizeImpl<TupleLike> {
    };

    /**
    * Specialization for volatile-qualified tuple-like objects
    * @tparam TupleLike The tuple-like type
    */
    template<typename TupleLike>
    struct TupleSize<volatile TupleLike> 
        : details::TupleSizeImpl<TupleLike> {
    };

    /**
    * Specialization for constvolatile-qualified tuple-like objects
    * @tparam TupleLike The tuple-like type
    */
    template<typename TupleLike>
    struct TupleSize<const volatile TupleLike> 
        : details::TupleSizeImpl<TupleLike> {
    };

    /** Equalivent of TupleSize<TupleLike>::Value. */
    template<typename TupleLike>
    inline constexpr usize TupleSizeV = TupleSize<TupleLike>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TUPLE_SIZE_H