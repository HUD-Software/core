#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_INTEGER_SEQUENCE_H
#define HD_INC_OSLAYER_TEMPLATES_INTEGER_SEQUENCE_H
#include "../Traits/IsIntegral.h"
#include "../Traits/CommonType.h"

namespace hud {

    /** Represents a compile-time sequence of integers. When used as an argument to a function template, the parameter pack Ints can be deduced and used in pack expansion. */
    template<typename T, T... ints>
    struct IntegerSequence {
        using Type = T;

        static_assert(IsIntegralV<T>, "hdIntegralSequence can only be instantiated with an integral type");
        static constexpr usize size() noexcept { return sizeof...(ints); }
    };

    /** Helper alias template defined for the common case where T is usize. */
    template<usize... ints>
    using IndexSequence = IntegerSequence<usize, ints...>;


    namespace details
    {
        // Stores a tuple of indices. Used by tuple and pair, and by bind() to
        // extract the elements in a tuple.
        template <usize... indices>
        struct IndexTuple {
            using Next = IndexTuple<indices..., sizeof...(indices)>;
        };

        // Builds an IndexTuple<0, 1, 2, ..., num-1>.
        template <usize num>
        struct BuildIndexTuple {
            using Type = typename BuildIndexTuple<num - 1u>::Type::Next;
        };
        template <>
        struct BuildIndexTuple<0u> {
            using Type = IndexTuple<>;
        };


        template < typename T, T num, typename Sequence = typename BuildIndexTuple<num>::Type>
        struct MakeIntegerSequence;
        template < typename T, T num, usize... int_index>
        struct MakeIntegerSequence < T, num, IndexTuple<int_index...>>
        {
            static_assert(num >= 0, "Cannot make integer sequence of negative length");
            using Type = IntegerSequence<T, static_cast<T>(int_index)...>;
        };

    }


    /** Helper alias template defined to simplify creation of IntegerSequence types with 0, 1, 2, ..., N-1 as Ints. */
    template < typename T, T num>
    using MakeIntegerSequence = typename details::MakeIntegerSequence<T, num>::Type;

    /** Helper alias template defined to simplify creation of IndexSequence types with 0, 1, 2, ..., N-1 as Ints. */
    template<usize num>
    using MakeIndexSequence = MakeIntegerSequence<usize, num>;

    /** Helper alias template IndexSequence is defined to convert any type parameter pack into an index sequence of the same length. */
    template<typename... TTypes>
    using MakeIndexSequenceFor = MakeIndexSequence<sizeof...(TTypes)>;

    /** Contenate IntegerSequences into one IntegerSequence */
    template<typename... IntSequences>
    struct CatIntegerSequence;

    template<typename T1, T1... seq_1, 
             typename T2, T2... seq_2, 
             typename... Rest>
    struct CatIntegerSequence<IntegerSequence<T1, seq_1...>, IntegerSequence<T2, seq_2...>, Rest...> { // Concatenate 2 IntegerSequence with the followings
        using Type = typename CatIntegerSequence<typename CatIntegerSequence<IntegerSequence<T1, seq_1...>, IntegerSequence<T2, seq_2...>>::Type, Rest...>::Type;
    };

    template<typename T, T... ints>
    struct CatIntegerSequence<IntegerSequence<T, ints...>> { // Concatenate 1 IntegerSequence
        using Type = IntegerSequence<T, ints...>;
    };
    template<typename T, T... seq_1, T... seq_2>
    struct CatIntegerSequence<IntegerSequence<T, seq_1...>, IntegerSequence<T, seq_2...>> { // Concantenante 2 IntegerSequence with same type
        using Type = IntegerSequence<T, seq_1..., seq_2...>;
    };
    template<typename T1, T1... seq_1, 
             typename T2, T2... seq_2>
    struct CatIntegerSequence<IntegerSequence<T1, seq_1...>, IntegerSequence<T2, seq_2...>> {  // Concantenante 2 IntegerSequence with different types
        using CommonT = CommonTypeT<T1, T2>;
        using Type = IntegerSequence<CommonT, seq_1..., seq_2...>;
    };

    /** Helper alias template equivalent to CatIntegerSequence<Sequences...>::Type. */
    template<typename... Sequences>
    using CatIntegerSequenceT = typename CatIntegerSequence<Sequences...>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_INTEGER_SEQUENCE_H
