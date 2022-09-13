#pragma once
#ifndef HD_INC_OSLAYER_TUPLE_H
#define HD_INC_OSLAYER_TUPLE_H
#include "../Minimal.h"
#include "TupleElement.h"
#include "TupleSize.h"

#include "../Templates/IntegerSequence.h"
#include "../Templates/Swap.h"
#include "../Templates/Forward.h"

#include "../Traits/IsSameSize.h"
#include "../Traits/EnableIf.h"
#include "../Traits/And.h"
#include "../Traits/Or.h"
#include "../Traits/Not.h"
#include "../Traits/IsDefaultConstructible.h"
#include "../Traits/IsImplicitlyDefaultConstructible.h"
#include "../Traits/IsConstructible.h"
#include "../Traits/IsConvertible.h"
#include "../Traits/IsNothrowDefaultConstructible.h"
#include "../Traits/IsNothrowCopyConstructible.h"
#include "../Traits/IsNothrowConstructible.h"
#include "../Traits/IsNothrowCopyAssignable.h"
#include "../Traits/IsCopyConstructible.h"
#include "../Traits/IsCopyAssignable.h"
#include "../Traits/IsMoveAssignable.h"
#include "../Traits/IsExplicitlyConstructible.h"
#include "../Traits/IsExplicitlyDefaultConstructible.h"
#include "../Traits/IsExplicitlyCopyConstructible.h"
#include "../Traits/IsExplicitlyMoveConstructible.h"
#include "../Traits/IsNotSame.h"
#include "../Traits/IsSwappable.h"
#include "../TagInit.h"

namespace hud {
    template<typename T1, typename T2>
    struct Pair;

    template<typename... Types>
    class Tuple;

    namespace details {

        /**
        * TupleLeaf is one tuple content associated with an index
        * @tparam leaf_index The index of the leaf
        * @tparam Type The type of the content
        */
        template<usize leaf_index, typename Type>
        struct TupleLeaf {

            /** Default constructor. Value-initialize content. */
            HD_FORCEINLINE constexpr TupleLeaf(TagInit) noexcept
                : content() {
                static_assert(IsNothrowDefaultConstructibleV<Type>, "Type() default constructor is throwable. Tuple is not designed to allow throwable default constructible components");
            }

            /** Default constructor. Do not initializes content. */
            HD_FORCEINLINE constexpr TupleLeaf() noexcept {
                static_assert(IsNothrowDefaultConstructibleV<Type>, "Type() default constructor is throwable. Tuple is not designed to allow throwable default constructible components");
            }

            /**
            * Initialization copy constructor.
            * @param arg Object to copy construct into the tuple leaf
            */
            HD_FORCEINLINE constexpr TupleLeaf(const Type& arg) noexcept
                : content(arg) {
                static_assert(IsNothrowCopyConstructibleV<Type>, "Type(const Type&) copy constructor is throwable. Tuple is not designed to allow throwable copy constructible components");
            }

            /**
            * Initialization move constructor.
            * @tparam UType Type of arg parameter
            * @param arg Object to move construct into the tuple leaf
            */
            template<typename UType>
            HD_FORCEINLINE constexpr TupleLeaf(UType&& arg) noexcept
                : content(forward<UType>(arg)) {
                static_assert(IsNothrowMoveConstructibleV<Type, UType>, "Type(UType&&) move constructor is throwable. Tuple is not designed to allow throwable move constructible components");
            }

            /**
            * Assigns operator.
            * @tparam UType Type of other TupleLeaf parameter
            * @param other Another tuple leaf
            * @return *this
            */
            template<typename UType>
            HD_FORCEINLINE constexpr TupleLeaf& operator=(UType&& arg) noexcept {
                static_assert(IsNothrowAssignableV<Type&, UType&&>, "Type& T::operator=(const UType&) is throwable. Tuple is not designed to allow throwable copy assignable components");
                content = forward<UType>(arg);
                return *this;
            }

            /** The element */
            Type content;
        };



        /**
        * TupleImpl is the implementation of the tuple with an sequence of index and a list of types
        * indices is used to access TupleLeaf that contains the Type where Types[indices]... relation is respected.
        * @tparam IndexSeq IndexSequence of Types...
        * @tparam Types... List of Types of the tuple
        */
        template<typename IndexSeq, typename... Types>
        struct TupleImpl;

        template<usize... indices, typename... Types>
        struct TupleImpl<IndexSequence<indices...>, Types...>
            : TupleLeaf<indices, Types>... {

            /**
            * Default constructor that calls all tuple leafs default constructors.
            * Value-initializes all elements, if any.
            */
            HD_FORCEINLINE constexpr TupleImpl(TagInit) noexcept
                : TupleLeaf<indices, Types>(taginit)... {
            }

            /**
            * Default constructor that calls all tuple leafs default constructors.
            * Do not initializes elements, if any.
            */
            HD_FORCEINLINE constexpr TupleImpl() noexcept
                : TupleLeaf<indices, Types>()... {
            }

            /**
            * Initialization copy constructor that calls all tuple leafs initialization copy constructors.
            * @param args List of objects to copy into the tuple
            */
            HD_FORCEINLINE constexpr TupleImpl(const Types&... args) noexcept
                : TupleLeaf<indices, Types>(args)...{
            }

            /**
            * Initialization move constructor that calls all tuple leafs initialization move constructors.
            * @param args List of objects to move into the tuple
            */
            template<typename... UTypes>
            HD_FORCEINLINE constexpr TupleImpl(UTypes&&... args) noexcept
                : TupleLeaf<indices, Types>(forward<UTypes>(args))...{
            }

            /** Copy constructor */
            constexpr TupleImpl(const TupleImpl&) = default;

            /** Move constructor */
            constexpr TupleImpl(TupleImpl&&) = default;
        };


        /**
        * Check if a Tuple<T0,T1> is explicitly copy constructible from Pair<F,S>.
        * In other words, if T0(const F{}) is explicit and T1(const S{}) is explicit
        */
        template<typename Pair, typename Tuple>
        struct IsPairExplictlyCopyConstructibleToTuple
            : FalseType {
        };
        template<typename F, typename S, typename T0, typename T1 >
        struct IsPairExplictlyCopyConstructibleToTuple<Pair<F, S>, Tuple<T0, T1>>
            : Or<IsExplicitlyCopyConstructible<T0, F>, IsExplicitlyCopyConstructible< T1, S >> {
        };
        template<typename Pair, typename Tuple>
        inline constexpr bool IsPairExplictlyCopyConstructibleToTupleV = IsPairExplictlyCopyConstructibleToTuple<Pair, Tuple>::Value;



        /**
        * Check if a Tuple<T0,T1> is explicitly move constructible from Pair<F,S>.
        * In other words, if T0(F{}) is explicit and T1(S{}) is explicit
        */
        template<typename Pair, typename Tuple>
        struct IsPairExplictlyMoveConstructibleToTuple
            : FalseType {
        };
        template<typename F, typename S, typename T0, typename T1 >
        struct IsPairExplictlyMoveConstructibleToTuple<Pair<F, S>, Tuple<T0, T1>>
            : Or<IsExplicitlyMoveConstructible<T0, F>, IsExplicitlyMoveConstructible< T1, S >> {
        };
        template<typename Pair, typename Tuple>
        inline constexpr bool IsPairExplictlyMoveConstructibleToTupleV = IsPairExplictlyMoveConstructibleToTuple<Pair, Tuple>::Value;

        /**
        * Recursively assign a tuple to another.
        * @tparam count Number of element to assign
        */
        template<usize count>
        struct TupleAssign {

            /**
            * Assign a 2 tuple elements.
            * @tparam Types... List of Types of the tuple to
            * @tparam UTypes... List of Types of the tuple from
            * @param to The assigned tuple
            * @param from The tuple to assign
            */
            template<typename... Types, typename... UTypes>
            constexpr void operator()([[maybe_unused]] Tuple<Types...>& to, [[maybe_unused]] const Tuple<UTypes...>& from) noexcept {
                static_assert(IsSameSizeV<Tuple<Types...>, Tuple<UTypes...>>, "Assigning tuples of different size is not supported");
                if constexpr (count > 0u) {
                    constexpr const usize idx = TupleSizeV< Tuple<Types...>>-count;
                    get<idx>(to) = get<idx>(from);
                    TupleAssign<count - 1u>()(to, from);
                }
            }

            /**
            * Assign a 2 tuple elements.
            * @tparam Types... List of Types of the tuple to
            * @tparam UTypes... List of Types of the tuple from
            * @param to The assigned tuple
            * @param from The tuple to assign
            */
            template<typename... Types, typename... UTypes>
            constexpr void operator()([[maybe_unused]] Tuple<Types...>& to, [[maybe_unused]] Tuple<UTypes...>&& from) noexcept {
                static_assert(IsSameSizeV<Tuple<Types...>, Tuple<UTypes...>>, "Assigning tuples of different size is not supported");
                if constexpr (count > 0) {
                    constexpr const usize idx = TupleSizeV< Tuple<Types...>>-count;
                    get<idx>(to) = get<idx>(move(from));
                    TupleAssign<count - 1u>()(to, move(from));
                }
            }
        };

        /** Swap TupleA and TupleB element at the index if element is swappable. */
        template<usize type_index, typename TupleA, typename TupleB, bool = hud::IsSwappableV<TupleElementT<type_index, TupleA>, TupleElementT<type_index, TupleB>>>
        struct SwapElement {
            template <typename>
            static constexpr bool EvaluateIfInstanciate = false;
            static_assert(EvaluateIfInstanciate<FalseType>, "Type is not swappable");
        };
        template<usize type_index, typename... Types, typename... UTypes >
        struct SwapElement < type_index, Tuple<Types...>, Tuple<UTypes...>, true> {
            constexpr void operator()(Tuple<Types...>& first, Tuple<UTypes...>& second) noexcept {
                hud::swap(get<type_index>(first), get<type_index>(second));
            }
        };

        /**
        * Recursively swap a tuple to another.
        * @tparam count Number of element to swap
        */
        template<usize count>
        struct TupleSwap {

            /**
            * Swap a 2 tuple elements.
            * @tparam Types... List of Types of the tuple first
            * @tparam UTypes... List of Types of the tuple second
            * @param first The first tuple to swap
            * @param second The second tulpe to swap
            */
            template<typename... Types, typename... UTypes>
            constexpr void operator()([[maybe_unused]] Tuple<Types...>& first, [[maybe_unused]] Tuple<UTypes...>& second) noexcept {
                static_assert(IsSameSizeV<Tuple<Types...>, Tuple<UTypes...>>, "Swapping tuples of different size is not supported");

                if constexpr (count > 0u) {
                    constexpr const usize index_to_swap = TupleSizeV< Tuple<Types...>>-count;
                    SwapElement<index_to_swap, Tuple<Types...>, Tuple<UTypes...>>()(first, second);

                    //hud::swap(get<index_to_swap>(first), get<index_to_swap>(second));
                    TupleSwap<count - 1u>()(first, second);
                }
            }
        };

        /**
        * Recursively compare a tuple to another.
        * @tparam count Number of element to compare
        */
        template<usize count>
        struct TupleEquals {
            /**
            * Compare a 2 tuple elements.
            * @tparam Types... List of Types of the tuple first
            * @tparam UTypes... List of Types of the tuple second
            * @param first The first tuple to compare
            * @param second The second tulpe to compare
            */
            template<typename... Types, typename... UTypes>
            [[nodiscard]]
            constexpr bool operator()([[maybe_unused]] const Tuple<Types...>& first, [[maybe_unused]] const Tuple<UTypes...>& second) noexcept {
                static_assert(IsSameSizeV<Tuple<Types...>, Tuple<UTypes...>>, "Comparing tuples of different size is not supported");
                if constexpr (count > 0u) {
                    constexpr const usize index_to_swap = TupleSizeV< Tuple<Types...>>-count;
                    return get<index_to_swap>(first) == get<index_to_swap>(second) && TupleEquals<count - 1u>()(first, second);
                }
                else {
                    return true;
                }
            }
        };

        /**
        * Recursively compare a tuple to another with operator<.
        * @tparam count Number of element to compare
        */
        template<usize count>
        struct TupleLess {
            /**
            * Compare a 2 tuple elements with operator<.
            * @tparam Types... List of Types of the tuple first
            * @tparam UTypes... List of Types of the tuple second
            * @param first The first tuple to compare
            * @param second The second tulpe to compare
            */
            template<typename... Types, typename... UTypes>
            [[nodiscard]]
            constexpr bool operator()([[maybe_unused]] const Tuple<Types...>& first, [[maybe_unused]] const Tuple<UTypes...>& second) noexcept {
                static_assert(hud::IsSameSizeV<Tuple<Types...>, Tuple<UTypes...>>, "Comparing tuples of different size is not supported");
                if constexpr (count > 0u) {
                    constexpr const usize index_to_swap = hud::TupleSizeV< Tuple<Types...>>-count;
                    if (get<index_to_swap>(first) < get<index_to_swap>(second)) {
                        return true;
                    }
                    if (get<index_to_swap>(second) < get<index_to_swap>(first)) {
                        return false;
                    }
                    return TupleLess<count - 1u>()(first, second);
                }
                else {
                    return false;
                }
            }
        };

        template<typename... TupleList>
        struct TupleCat {

            template<typename... Tuples>
            struct TupleCatImpl {

                /**
                * Create a new tuple equivalent to all input tuples arguments inside a single tuple.
                * For exemple:
                *     If Tuples... is Tuple<u32, f32>, Tuple<char, wchar, char16>, Tuple<u64, f64>
                *     TupleType is Tuple<u32, f32, char, wchar, char16, u64 f64>
                */
                template< typename Tuple, typename... Rest >
                struct CatTuplesArg {
                    using TupleType = Tuple;
                };
                template< typename... Args1, typename... Args2, typename... Rest>
                struct CatTuplesArg< Tuple < Args1...>, Tuple < Args2...>, Rest...> {
                    using TupleType = typename CatTuplesArg< Tuple<Args1..., Args2...>, Rest...>::TupleType;
                };
                using ReturnType = typename CatTuplesArg<Tuples...>::TupleType;

                /**
                * Create an IndexSequence of the Tuples... list element index.
                * For exemple:
                *     If Tuples... is Tuple<u32, f32>, Tuple<char, wchar, char16>, Tuple<u64, f64>
                *     TupleType is Tuple<u32, f32, char, wchar, char16, u64 f64>
                *     ElementIndexSeq is IndexSequence< 0,1, 0,1,2, 0,1>
                */
                template< typename ElementIndicesSeq, typename...>
                struct CatElementIndex {
                    using ElementIndexSeq = ElementIndicesSeq;
                };
                template<usize... element_indices, typename... Args, typename... Rest>
                struct CatElementIndex< IndexSequence<element_indices...>, Tuple<Args...>, Rest...> {
                    using ElementIndexSeq = typename CatElementIndex<CatIntegerSequenceT< IndexSequence<element_indices...>, MakeIndexSequenceFor<Args...>>, Rest...>::ElementIndexSeq;
                };
                using ElementIndexSeq = typename CatElementIndex<MakeIndexSequenceFor<>, Tuples...>::ElementIndexSeq;

                /**
                * Create a IndexSequence of the Tuples... list index mask to match the element index IndexSequence.
                * For exemple:
                *     If Tuples... is Tuple<u32, f32>, Tuple<char, wchar, char16>, Tuple<u64, f64>
                *     TupleType is Tuple<u32, f32, char, wchar, char16, u64 f64>
                *     ElementIndexSeq is IndexSequence< 0,1, 0,1,2, 0,1>
                *     MaskIndexSeq is IndexSequence< 0,0, 1,1,1, 2,2>
                */
                template<usize mask_index, typename T>
                static constexpr usize repeat_mask_index = mask_index;

                template< usize tuple_index, typename MaskSeq, typename...Rest >
                struct CatMaskIndex {
                    using MaskIndexSeq = MaskSeq;
                    static_assert(sizeof...(Rest) == 0, "Unsupported tuple_cat arguments.");
                };
                template<usize tuple_index, usize... mask_indices, typename... Args, typename... Rest>
                struct CatMaskIndex<tuple_index, IndexSequence<mask_indices...>, Tuple<Args...>, Rest... > {
                    using MaskIndexSeq = typename CatMaskIndex< tuple_index + 1u, CatIntegerSequenceT< IndexSequence<mask_indices...>, IndexSequence<repeat_mask_index<tuple_index, Args>...>>, Rest... >::MaskIndexSeq;
                };
                using MaskIndexSeq = typename CatMaskIndex<0, MakeIndexSequenceFor<>, Tuples...>::MaskIndexSeq;

                /**
                * Constructs a tuple that is a concatenation of all tuples in the given "tuple of tulpes" in the same order.
                * @tparam Tuple The Tuple type of tuples to concatenate
                * @tparam element_indices List element index
                * @tparam mask_indices List index mask to match the element index IndexSequence
                * @param IndexSequence of element_indices
                * @param IndexSequence of mask_indices
                * @param tulpe The tuple of tulpes to concatenate
                * @return The concatenated tuple
                */
                template<typename Tuple, usize... element_indices, usize... mask_indices>
                static HD_FORCEINLINE constexpr ReturnType concatenate(IndexSequence<element_indices...>, IndexSequence<mask_indices...>, Tuple&& tuple) noexcept {
                    return ReturnType(get<element_indices>(get<mask_indices>(forward<Tuple>(tuple)))...);
                }
            };

            template<>
            struct TupleCatImpl<> {
                using ReturnType = Tuple<>;
                using ElementIndexSeq = IndexSequence<>;
                using MaskIndexSeq = IndexSequence<>;

                /**
                * Constructs an empty tuple
                * @param tulpe The tuple of tulpes to concatenate
                * @return The concatenated tuple
                */
                template<typename EmptyTuple, usize... element_indices, usize... mask_indices>
                static HD_FORCEINLINE constexpr EmptyTuple concatenate(IndexSequence<element_indices...>, IndexSequence<mask_indices...>, EmptyTuple&&) noexcept {
                    return EmptyTuple();
                }
            };

            using TupleCarImpl = TupleCatImpl<DecayT<TupleList>...>;
            using ReturnType = typename TupleCarImpl::ReturnType;
            using ElementIndexSeq = typename TupleCarImpl::ElementIndexSeq;
            using MaskIndexSeq = typename TupleCarImpl::MaskIndexSeq;

            /**
            * Constructs a tuple that is a concatenation of all tuples in the given "tuple of tulpes" in the same order.
            * @tparam Tuple The Tuple type of tuples to concatenate
            * @param tulpe The tuple of tulpes to concatenate
            * @return The concatenated tuple
            */
            template<typename Tuple>
            static HD_FORCEINLINE constexpr ReturnType concatenate(Tuple&& tuple) noexcept {
                return TupleCarImpl::concatenate(ElementIndexSeq(), MaskIndexSeq(), forward<Tuple>(tuple));
            }
        };
    }


    /**
    * Tuples are objects that pack elements of possibly different types together in a single object, just like pair objects do for pairs of elements, but generalized for any number of elements.
    * Conceptually, they are similar to plain old data structures (C-like structs) but instead of having named data members, its elements are accessed by their order in the tuple.
    * The selection of particular elements within a tuple is done at the template-instantiation level, and thus, it must be specified at compile-time, with helper functions.
    * The tuple class is closely related to the Pair class: Tuples can be constructed from pairs, and pairs can be treated as tuples for certain purposes.
    * @tparam Types... List of types of the tuple
    */
    template<typename... Types>
    class Tuple 
        : details::TupleImpl<MakeIndexSequenceFor<Types...>, Types...> {

    private:
        using Super = details::TupleImpl<MakeIndexSequenceFor<Types...>, Types...>;

    public:
        /**
        * Default constructor.
        * This involves individually value-initializes all elements, if any.
        * Tuple do not accept throwable default constructible components
        */
        explicit(OrV<IsExplicitlyDefaultConstructible<Types>...>)
            constexpr Tuple(TagInit) noexcept requires(AndV<IsDefaultConstructible<Types>...>)
            : Super(taginit) {
        }

        /**
        * Default constructor
        * This involves individually default constructs all components, with an initialization that depends on the constructor.
        * Constructs a tuple without value-initializing trivially default constructible elements.
        * Tuple do not accept throwable default constructible components
        */
        explicit(OrV<IsExplicitlyDefaultConstructible<Types>...>)
            constexpr Tuple() noexcept requires(AndV<IsDefaultConstructible<Types>...>) {
        }

        /**
        Initialization copy constructor.
        This involves individually copy constructs all components, with an initialization that depends on the constructor.
        Tuple do not accept throwable copy constructible components.
        @param args List of objects to copy construct into the tuple
        */
        template<typename... UTypes>
        explicit(OrV<IsExplicitlyCopyConstructible<Types, UTypes>...>)
            constexpr Tuple(const UTypes&... args) noexcept
            : Super(args...) {
        }

        /**
        * Initialization move constructor.
        * This involves individually move constructs all components, with an initialization that depends on the constructor.
        * Tuple do not accept throwable move constructible components.
        * @param args List of objects to move construct into the tuple
        */
        template<typename... UTypes>
        explicit(OrV<IsExplicitlyMoveConstructible<Types, UTypes>...>)
            constexpr Tuple(UTypes&&... args) noexcept
            : Super(forward<UTypes>(args)...) {
        }


        /**
        * Initialization copy constructor from a Pair.
        * This involves individually copy constructs Pair elements in Tuple, with an initialization that depends on the constructor.
        * Tuple do not accept throwable copy constructible components.
        * @tparam F Type of the first component of Pair
        * @tparam S Type of the second component of Pair
        * @param pair The Pair to copy construct
        */
        template<typename F, typename S>
        explicit(details::IsPairExplictlyCopyConstructibleToTupleV<Pair<F, S>, Tuple>)
            constexpr Tuple(const Pair< F, S >& pair) noexcept
            : Tuple(pair.first, pair.second) {
        }

        /**
        * Initialization move constructor from a Pair.
        * This involves individually move constructs Pair elements in Tuple, with an initialization that depends on the constructor.
        * Tuple do not accept throwable move constructible components.
        * @tparam F Type of the first component of Pair
        * @tparam S Type of the second component of Pair
        * @param pair The Pair to copy construct
        */
        template<typename F, typename S>
        explicit(details::IsPairExplictlyMoveConstructibleToTupleV<Pair<F, S>, Tuple>)
            constexpr Tuple(Pair< F, S >&& pair) noexcept
            : Tuple(move(pair.first), move(pair.second)) {
        }

        /**
        * Copy constructor.
        * The object is initialized with the contents of the other Tuple elements.
        * The corresponding members of other is passed to the copy constructor of each of its members.
        * Tuple do not accept throwable copy constructible components.
        * @param other Another tuple object.
        */
        constexpr Tuple(const Tuple& other) noexcept requires(AndV<IsCopyConstructible<Types>...>) = default;

        /**
        * Copy constructor.
        * The object is initialized with the contents of the other tuple object.
        * The corresponding members of other is passed to the copy constructor of each of its members.
        * Tuple do not accept throwable copy constructible components.
        * @tparam UTypes... List of types of the tuple to copy
        * @param other Another tuple object to copy
        */
        template<typename... UTypes>
        explicit(OrV<IsExplicitlyCopyConstructible<Types, UTypes>...>)
            constexpr Tuple(const Tuple<UTypes...>& other) noexcept requires(AndV<IsSameSize<Tuple, Tuple<UTypes...>>, IsCopyConstructible<Types>...>)
            : Tuple(other, MakeIndexSequenceFor<UTypes...>{}) {
        }


        /**
        * Move constructor.
        * The object is initialized with the contents of the other Tuple elements.
        * The corresponding members of other is forward to the move constructor of each of its members.
        * Tuple do not accept throwable move constructible components.
        * @param other Another tuple object to move
        */
        constexpr Tuple(Tuple&& other) noexcept = default;

        /**
        * Move constructor.
        * The object is initialized with the contents of the other Tuple elements.
        * The corresponding members of other is forward to the move constructor of each of its members.
        * Tuple do not accept throwable move constructible components.
        * @param other Another tuple object to move
        */
        template<typename... UTypes>
        explicit(OrV<IsExplicitlyMoveConstructible<Types, UTypes>...>)
            constexpr Tuple(Tuple<UTypes...>&& other) noexcept
            : Tuple(forward<Tuple<UTypes...>>(other), MakeIndexSequenceFor<UTypes...>{}) {
        }


        /**
        * Assigns operator.
        * Perform copy assignments, with the elements of its argument preserving their values after the call.
        * Tuple do not accept throwable copy assignable components.
        * @param other Another tuple to assign
        * @return *this
        */
        constexpr Tuple& operator=(const Tuple& other) noexcept requires(AndV<IsCopyAssignable<Types>...>) {
            details::TupleAssign<sizeof...(Types)>()(*this, other);
            return *this;
        }

        /**
        * Assigns operator.
        * Perform copy assignments, with the elements of its argument preserving their values after the call.
        * Tuple do not accept throwable copy assignable components.
        * @param other Another tuple to assign
        * @return *this
        */
        template<typename... UTypes>
        constexpr Tuple& operator=(const Tuple<UTypes...>& other) noexcept requires(AndV<IsCopyAssignable<Types, UTypes>...>) {
            details::TupleAssign<sizeof...(Types)>()(*this, other);
            return *this;
        }

        /**
        * Assigns other as the new content for the tuple object.
        * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
        * Tupel do not accept throwable move assignable components.
        * @param other Another tuple object.
        * @return *this
        */
        constexpr Tuple& operator=(Tuple&& other) noexcept requires(AndV<IsMoveAssignable<Types>...>) {
            details::TupleAssign<sizeof...(Types)>()(*this, move(other));
            return *this;
        }

        /**
        * Assigns other as the new content for the tuple object.
        * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
        * Tupel do not accept throwable move assignable components.
        * @param other Another tuple object.
        * @return *this
        */
        template<typename... UTypes>
        constexpr Tuple& operator=(Tuple<UTypes...>&& other) noexcept requires(AndV<IsMoveAssignable<Types, UTypes>...>) {
            details::TupleAssign<sizeof...(Types)>()(*this, move(other));
            return *this;
        }

        template<usize element_index, typename... UTypes>
        friend constexpr TupleElementT<element_index, Tuple<UTypes...>>& get(Tuple<UTypes...>& tuple) noexcept;

        template<usize element_index, typename... UTypes>
        friend constexpr const TupleElementT<element_index, Tuple<UTypes...>>& get(const Tuple<UTypes...>& tuple) noexcept;

        template<usize element_index, typename... UTypes>
        friend constexpr TupleElementT<element_index, Tuple<UTypes...>>&& get(Tuple<UTypes...>&& tuple) noexcept;

        template<usize element_index, typename... UTypes>
        friend constexpr const TupleElementT<element_index, Tuple<UTypes...>>&& get(const Tuple<UTypes...>&& tuple) noexcept;

    private:
        /**
        * Copy construct a Tuple by unwrapping Tuple element to copy and call initialisation constructor with unwrapped elements as parameter
        * @tparam TupleT The type of Tuple to copy
        * @tparam indices... The tuple element indices
        * @param tuple The Tuple to copy
        * @param indices... The IndexSequence of indices
        */
        template<typename TupleT, usize... indices>
        HD_FORCEINLINE constexpr explicit Tuple(TupleT&& tuple, IndexSequence<indices...>) noexcept
            : Tuple(get<indices>(forward<TupleT>(tuple))...) {
        }
    };

    /** Specialization for empty tuple. */
    template<>
    class Tuple<> {
    };

    /**
    * IsSameSize trait specialization for Tuple.
    * @tparam Types... List of Types of the first tuple
    * @tparam Types2... List of Types of the second tuple
    */
    template<typename... Types, typename... Types2>
    struct IsSameSize<Tuple<Types...>, Tuple<Types2...>>
        : BoolConstant<sizeof...(Types) == sizeof...(Types2)> {
    };

    /**
    * TupleSize specialization for Tuple
    * @tparam Types... List of Types of the tuple
    */
    template<typename... Types>
    struct TupleSize < Tuple<Types...>> 
        : IntegralConstant<usize, sizeof...(Types)> {
    };

    /**
    * TupleElement specilization for Tuple
    * @tparam index Index of the type to reach
    * @tparam T Current type
    * @tparam Rest Types after T in Tuple
    */
    template<usize type_index>
    struct TupleElement<type_index, Tuple<>> {
        // Out of bound index
        template <typename>
        static constexpr bool EvaluateIfInstanciate = false;
        static_assert(EvaluateIfInstanciate<IntegralConstant<usize, type_index>>, "Tuple index out of bounds");
    };

    template<typename T, typename... Rest>
    struct TupleElement < 0, Tuple<T, Rest...>> {
        // Reach given index, reach the index-th Type
        using Type = T;
    };

    template<usize type_index, typename T, typename... Rest>
    struct TupleElement<type_index, Tuple<T, Rest...>>
        : TupleElement<type_index - 1, Tuple<Rest...>> {
        // Recursive definition, when reach 0 Type is the one define above
    };


    /**
    * Retrieves a lvalue reference to the member of a tuple at an index
    * @tparam idx_to_reach The index to reach
    * @tparam Types... Types of the tuple
    * @param tuple The tuple to access
    * @return LValue reference to the member mFirst if index is 0, mSecond if index is 1.
    */
    template<usize idx_to_reach, typename... Types>
    [[nodiscard]]
    HD_FORCEINLINE constexpr TupleElementT<idx_to_reach, Tuple<Types...>>& get(Tuple<Types...>& tuple) noexcept {
        using Type = TupleElementT<idx_to_reach, Tuple<Types...>>;
        return static_cast<details::TupleLeaf<idx_to_reach, Type>&>(tuple).content;
    }

    /**
    * Retrieves a lvalue reference to the member of a tuple at an index
    * @tparam idx_to_reach The index to reach
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param pair The pair to access
    * @return LValue reference to the member mFirst if index is 0, mSecond if index is 1.
    */
    template<usize idx_to_reach, typename... Types>
    [[nodiscard]]
    HD_FORCEINLINE constexpr const TupleElementT<idx_to_reach, Tuple<Types...>>& get(const Tuple<Types...>& tuple) noexcept {
        using Type = TupleElementT<idx_to_reach, Tuple<Types...>>;
        return static_cast<const details::TupleLeaf<idx_to_reach, Type>&>(tuple).content;
    }

    /**
    * Retrieves a rvalue reference to the member of a tuple at an index
    * @tparam idx_to_reach The index to reach
    * @tparam Types... Types of the tuple
    * @param tuple The tuple to access
    * @return RValue reference to the member mFirst if index is 0, mSecond if index is 1.
    */
    template<usize idx_to_reach, typename... Types>
    [[nodiscard]]
    HD_FORCEINLINE constexpr TupleElementT<idx_to_reach, Tuple<Types...>>&& get(Tuple<Types...>&& tuple) noexcept {
        using Type = TupleElementT<idx_to_reach, Tuple<Types...>>;
        return forward<Type>(static_cast<details::TupleLeaf<idx_to_reach, Type>&&>(tuple).content);
    }

    /**
    * Retrieves a rvalue reference to the member of a tuple at an index
    * @tparam idx_to_reach The index to reach
    * @tparam Types... Types of the tuple
    * @param tuple The tuple to access
    * @return RValue reference to the member mFirst if index is 0, mSecond if index is 1.
    */
    template<usize idx_to_reach, typename... Types>
    [[nodiscard]]
    HD_FORCEINLINE constexpr const TupleElementT<idx_to_reach, Tuple<Types...>>&& get(const Tuple<Types...>&& tuple) noexcept {
        using Type = TupleElementT<idx_to_reach, Tuple<Types...>>;
        return forward<const Type>(static_cast<const details::TupleLeaf<idx_to_reach, Type>&&>(tuple).content);
    }

    /**
    * swap specialization for Tuple
    * @tparam Types Types of the tuple
    * @param first The first tuple to swap
    * @param second The second tuple to swap
    */
    template<typename... Types, typename... UTypes>
    static constexpr void swap(Tuple<Types...>& first, Tuple<UTypes...>& second) noexcept {
        details::TupleSwap<sizeof...(Types)>()(first, second);
    }

    /**
    * Checks if all elements of left tuple are equals to all elements at the same index of right tuple, that is, compares all elements by operator==.
    * @tparam Types Types of the left tuple
    * @tparam UTypes Types of the right tuple
    * @param first The first tuple to compare by operator==
    * @param second The second tuple to compareby operator==
    * @return true if all elements of left tuple are equals to all elements at the same index of right tuple, false otherwise
    */
    template<typename... Types, typename... UTypes>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator==(const Tuple<Types...>& left, const Tuple<UTypes...>& right) noexcept {
        return details::TupleEquals<sizeof...(Types)>()(left, right);
    }

    /**
    * Checks if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, that is, compares all elements by operator==.
    * @tparam Types Types of the left tuple
    * @tparam UTypes Types of the right tuple
    * @param first The first tuple to compare by operator==
    * @param second The second tuple to compareby operator==
    * @return true if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, false otherwise
    */
    template<typename... Types, typename... UTypes>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator!=(const Tuple<Types...>& left, const Tuple<UTypes...>& right) noexcept {
        return !(left == right);
    }

    /**
    * Checks if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, that is, compares all elements by operator==.
    * @tparam Types Types of the tuple
    * @param first The first tuple to compare by operator==
    * @param second The second tuple to compareby operator==
    * @return true if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, false otherwise
    */
    template<typename... Types, typename... UTypes>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<(const Tuple<Types...>& left, const Tuple<UTypes...>& right) noexcept {
        return details::TupleLess<sizeof...(Types)>()(left, right);
    }

    /**
    * Lexicographically compares all elements of left tuple are greater than all elements at the same index of right tuple, that is, compares all elements by operator<.
    * @tparam Types Types of the tuple
    * @param first The first tuple to compare by operator<
    * @param second The second tuple to compare by operator<
    * @return true if all elements of left tuple are greater than all elements at the same index of right tuple, false otherwise
    */
    template<typename... Types, typename... UTypes>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>(const Tuple<Types...>& left, const Tuple<UTypes...>& right) noexcept {
        return right < left;
    }

    /**
    * Lexicographically compares all elements of this tuple are less or equals all elements at the same index of another tuple, that is, compares all elements by operator<.
    * @tparam Types Types of the tuple
    * @param first The first tuple to compare by operator<
    * @param second The second tuple to compare by operator<
    * @return true all elements of this tuple is lexicographically less or equals the other elements at the same index, false otherwise
    */
    template<typename... Types, typename... UTypes>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<=(const Tuple<Types...>& left, const Tuple<UTypes...>& right) noexcept {
        return !(left > right);
    }

    /**
    * Lexicographically compares all elements of left tuple are greater or equals than all elements at the same index of right tuple, that is, compares all elements by operator<.
    * @tparam Types Types of the tuple
    * @param first The first tuple to compare by operator<
    * @param second The second tuple to compare by operator<
    * @return true if all elements of left tuple are greater or equals than all elements at the same index of right tuple, false otherwise
    */
    template<typename... Types, typename... UTypes>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>=(const Tuple<Types...>& left, const Tuple<UTypes...>& right) noexcept {
        return !(left < right);

    }
    /**
    * Creates a Tuple object, deducing the target type from the types of arguments.
    * @tparam Types List of tuple types
    * @param args Tuple constructor arguments list
    * @return Tuple<Types...> instance.
    */
    template< typename... Types >
    [[nodiscard]]
    HD_FORCEINLINE constexpr Tuple<Types...> make_tuple(Types&&... args) noexcept {
        return Tuple<Types...>(forward<Types>(args)...);
    }

    /**
    * Constructs a tuple object with rvalue references to the elements in args suitable to be forwarded as argument to a function.
    * This function is designed to forward arguments, not to store its result in a named variable, since the returned object may contain references to temporary variables.
    */
    template<typename... Types>
    HD_FORCEINLINE constexpr Tuple<Types&&...> forward_as_tuple(Types&&... args) noexcept {
        return Tuple<Types&&...>(forward<Types>(args)...);
    }

    /**
    * Constructs a tuple that is a concatenation of all tuples in args in the same order.
    * @tparam Tuples All Tuple types
    * @param args Zero or more tuple to concatenate
    * @return The concatenated tuple
    */
    template<typename...Tuples>
    HD_FORCEINLINE constexpr typename details::TupleCat<Tuples...>::ReturnType tuple_cat(Tuples&&... args) noexcept {
        using TupleCatResult = details::TupleCat<Tuples...>;
        return TupleCatResult::concatenate(forward_as_tuple(forward<Tuples>(args)...));
    }

} // namespace hud

#endif // HD_INC_OSLAYER_TUPLE_H