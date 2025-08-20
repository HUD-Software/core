#ifndef HD_INC_CORE_COMPRESSED_TUPLE_H
#define HD_INC_CORE_COMPRESSED_TUPLE_H
#include "tuple.h"
#include "../traits/is_empty.h"

namespace hud
{
    template<typename type1_t, typename type2_t>
    struct pair;

    template<typename... types_t>
    class compressed_tuple;

    template<usize idx_to_reach, typename... types_t>
    constexpr tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &get(compressed_tuple<types_t...> &t) noexcept;
    template<usize idx_to_reach, typename... types_t>
    constexpr const tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &get(const compressed_tuple<types_t...> &t) noexcept;
    template<usize idx_to_reach, typename... types_t>
    constexpr tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &&get(compressed_tuple<types_t...> &&t) noexcept;
    template<usize idx_to_reach, typename... types_t>
    constexpr const tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &&get(const compressed_tuple<types_t...> &&t) noexcept;

    namespace details::tuple
    {
        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_copy_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::compressed_tuple<tuple_type0_t, tuple_type1_t>>
            : hud::conjunction<
                  hud::is_copy_constructible<tuple_type0_t, pair_type0_t>,
                  hud::is_copy_constructible<tuple_type1_t, pair_type1_t>>
        {
        };

        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_explicitly_copy_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::compressed_tuple<tuple_type0_t, tuple_type1_t>>
            : hud::disjunction<hud::is_explicitly_copy_constructible<tuple_type0_t, pair_type0_t>, hud::is_explicitly_copy_constructible<tuple_type1_t, pair_type1_t>>
        {
        };

        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_move_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::compressed_tuple<tuple_type0_t, tuple_type1_t>>
            : hud::conjunction<
                  hud::is_move_constructible<tuple_type0_t, pair_type0_t>,
                  hud::is_move_constructible<tuple_type1_t, pair_type1_t>>
        {
        };

        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_explicitly_move_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::compressed_tuple<tuple_type0_t, tuple_type1_t>>
            : hud::disjunction<hud::is_explicitly_move_constructible<tuple_type0_t, pair_type0_t>, hud::is_explicitly_move_constructible<tuple_type1_t, pair_type1_t>>
        {
        };
    } // namespace details::tuple

    namespace details::compressed_tuple
    {

        /**
         * compressed_tuple_leaf is one compressed_tuple content associated with an index
         * @tparam leaf_index The index of the leaf
         * @tparam type_t The type of the content
         */
        template<usize leaf_index, typename type_t>
        struct compressed_tuple_leaf
            : type_t
        {

            /** Default constructor. Value-initialize content. */
            HD_FORCEINLINE constexpr compressed_tuple_leaf(hud::tag_init_t) noexcept
                : type_t()
            {
                static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t() default constructor is throwable. compressed_tuple is not designed to allow throwable default constructible components");
            }

            /** Default constructor. Do not initializes content. */
            HD_FORCEINLINE constexpr compressed_tuple_leaf() noexcept
                : type_t()
            {
                static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t() default constructor is throwable. compressed_tuple is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization copy constructor.
             * @param arg Object to copy construct into the compressed_tuple leaf
             */
            HD_FORCEINLINE constexpr compressed_tuple_leaf(const type_t &arg) noexcept
                : type_t(arg)
            {
                static_assert(hud::is_nothrow_copy_constructible_v<type_t>, "type_t(const type_t&) copy constructor is throwable. compressed_tuple is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * @tparam UType type_t of arg parameter
             * @param arg Object to move construct into the compressed_tuple leaf
             */
            template<typename UType>
            requires(hud::is_constructible_v<type_t, UType>)
            HD_FORCEINLINE constexpr compressed_tuple_leaf(UType &&arg) noexcept
                : type_t(hud::forward<UType>(arg))
            {
                static_assert(hud::is_nothrow_move_constructible_v<type_t, UType>, "type_t(UType&&) move constructor is throwable. compressed_tuple is not designed to allow throwable move constructible components");
            }

            /**
             * Piecewise constructor forwarding a compressed_tuple of arguments.
             * @tparam Args Types of the elements in the compressed_tuple used for construction.
             * @param compressed_tuple Tuple of arguments to forward to the constructor of type_t.
             */
            template<typename... Args>
            constexpr compressed_tuple_leaf(hud::tag_piecewise_construct_t, hud::compressed_tuple<Args...> &&compressed_tuple) noexcept
                : compressed_tuple_leaf(hud::forward<hud::compressed_tuple<Args...>>(compressed_tuple), hud::make_index_sequence_for<Args...> {})
            {
            }

            /**
             * Piecewise constructor that unpacks a compressed_tuple into individual arguments.
             *
             * @tparam Args Types of the elements in the compressed_tuple used for construction.
             * @tparam indices Index sequence used to extract each element from the compressed_tuple.
             * @param compressed_tuple Tuple whose elements are forwarded to the constructor of type_t.
             */
            template<typename... Args, usize... indices>
            constexpr compressed_tuple_leaf(hud::compressed_tuple<Args...> &&compressed_tuple, hud::index_sequence<indices...>) noexcept
                : type_t(hud::forward<Args>(hud::get<indices>(compressed_tuple))...)
            {
                static_assert(hud::is_nothrow_constructible_v<type_t, Args...>, "type_t(Args&&...) constructor is throwable. pair is not designed to allow throwable constructible components");
            }

            /**
             * Assigns operator.
             * @tparam UType type_t of other compressed_tuple_leaf parameter
             * @param other Another compressed_tuple leaf
             * @return *this
             */
            template<typename UType>
            HD_FORCEINLINE constexpr compressed_tuple_leaf &operator=(UType &&arg) noexcept
            {
                static_assert(hud::is_nothrow_assignable_v<type_t &, UType &&>, "type_t& type_t::operator=(const UType&) is throwable. compressed_tuple is not designed to allow throwable copy assignable components");
                static_cast<type_t &>(*this) = hud::forward<UType>(arg);
                return *this;
            }

            /** Retrivies the contained element */
            [[nodiscard]] constexpr type_t &element() & noexcept
            {
                return *this;
            }

            /** Retrivies the contained element */
            [[nodiscard]] constexpr const type_t &element() const & noexcept
            {
                return *this;
            }

            /** Retrivies the contained element */
            [[nodiscard]] constexpr const type_t &&element() const && noexcept
            {
                return hud::move(*this);
            }

            /** Retrivies the contained element */
            [[nodiscard]] constexpr type_t &&element() && noexcept
            {
                return hud::move(*this);
            }
        };

        template<usize leaf_index, typename type_t, bool = hud::is_empty_v<type_t>>
        struct tuple_leaf_select;

        template<usize leaf_index, typename type_t>
        struct tuple_leaf_select<leaf_index, type_t, true>
        {
            using type = compressed_tuple_leaf<leaf_index, type_t>;
        };

        template<usize leaf_index, typename type_t>
        struct tuple_leaf_select<leaf_index, type_t, false>
        {
            using type = hud::details::tuple::tuple_leaf<leaf_index, type_t>;
        };

        /**
         * compressed_tuple_impl is the implementation of the compressed_tuple with an sequence of index and a list of types
         * indices is used to access tuple_leaf that contains the type_t where types_t[indices]... relation is respected.
         * @tparam index_seq_t hud::index_sequence of types_t...
         * @tparam types_t... List of types_t of the compressed_tuple
         */
        template<typename index_seq_t, typename... types_t>
        struct compressed_tuple_impl;

        /** We need this to force MSVC to do EBCO of multiple inheritance. */
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_MSVC)
    #define EBCO_MSVC __declspec(empty_bases)
#else
    #define EBCO_MSVC
#endif

        template<usize... indices, typename... types_t>
        struct EBCO_MSVC compressed_tuple_impl<index_sequence<indices...>, types_t...>
            : tuple_leaf_select<indices, types_t>::type...
        {

            /**
             * Default constructor that calls all compressed_tuple leafs default constructors.
             * Value-initializes all elements, if any.
             */
            constexpr compressed_tuple_impl(tag_init_t) noexcept
                : tuple_leaf_select<indices, types_t>::type(hud::tag_init)...
            {
            }

            /**
             * Default constructor that calls all compressed_tuple leafs default constructors.
             * Do not initializes elements, if any.
             */
            constexpr compressed_tuple_impl() noexcept
                : tuple_leaf_select<indices, types_t>::type()...
            {
            }

            /**
             * Initialization copy constructor that calls all compressed_tuple leafs initialization copy constructors.
             * @param args List of objects to copy into the compressed_tuple
             */
            constexpr compressed_tuple_impl(const types_t &...args) noexcept
                : tuple_leaf_select<indices, types_t>::type(args)...
            {
            }

            /**
             * Initialization move constructor that calls all compressed_tuple leafs initialization move constructors.
             * @param args List of objects to move into the compressed_tuple
             */
            template<typename... u_types_t>
            constexpr compressed_tuple_impl(u_types_t &&...args) noexcept
                : tuple_leaf_select<indices, types_t>::type(hud::forward<u_types_t>(args))...
            {
            }

            /**
             * Piecewise constructor for `compressed_tuple` using tuples of arguments to construct each compressed_tuple leafs.
             * @param hud::tag_piecewise_construct_t Tag to indicate piecewise construction.
             * @param tuples Tuples containing arguments to forward to the constructor of each leafs.
             */
            template<typename... tuple_types>
            constexpr compressed_tuple_impl(hud::tag_piecewise_construct_t, tuple_types &&...tuples) noexcept
                : tuple_leaf_select<indices, types_t>::type(hud::tag_piecewise_construct, hud::forward<tuple_types>(tuples))...
            {
            }

            /** Copy constructor */
            constexpr compressed_tuple_impl(const compressed_tuple_impl &) = default;

            /** Move constructor */
            constexpr compressed_tuple_impl(compressed_tuple_impl &&) = default;
        };

        /**
         * Recursively assign a compressed_tuple to another.
         * @tparam count Number of element to assign
         */
        template<usize count>
        struct tuple_assign
        {
            /**
             * Assign a 2 compressed_tuple elements.
             * @tparam types_t... List of types_t of the compressed_tuple to
             * @tparam u_types_t... List of types_t of the compressed_tuple from
             * @param to The assigned compressed_tuple
             * @param from The compressed_tuple to assign
             */
            template<typename... types_t, typename... u_types_t>
            constexpr void operator()([[maybe_unused]] hud::compressed_tuple<types_t...> &to, [[maybe_unused]] const hud::compressed_tuple<u_types_t...> &from) noexcept
            {
                static_assert(hud::is_same_size_v<hud::compressed_tuple<types_t...>, hud::compressed_tuple<u_types_t...>>, "Assigning tuples of different size is not supported");
                if constexpr (count > 0u)
                {
                    constexpr const usize idx = tuple_size_v<hud::compressed_tuple<types_t...>> - count;
                    hud::get<idx>(to) = hud::get<idx>(from);
                    tuple_assign<count - 1u>()(to, from);
                }
            }

            /**
             * Assign a 2 compressed_tuple elements.
             * @tparam types_t... List of types_t of the compressed_tuple to
             * @tparam u_types_t... List of types_t of the compressed_tuple from
             * @param to The assigned compressed_tuple
             * @param from The compressed_tuple to assign
             */
            template<typename... types_t, typename... u_types_t>
            constexpr void operator()([[maybe_unused]] hud::compressed_tuple<types_t...> &to, [[maybe_unused]] hud::compressed_tuple<u_types_t...> &&from) noexcept
            {
                static_assert(hud::is_same_size_v<hud::compressed_tuple<types_t...>, hud::compressed_tuple<u_types_t...>>, "Assigning tuples of different size is not supported");
                if constexpr (count > 0)
                {
                    constexpr const usize idx = tuple_size_v<hud::compressed_tuple<types_t...>> - count;
                    hud::get<idx>(to) = hud::get<idx>(hud::move(from));
                    tuple_assign<count - 1u>()(to, hud::move(from));
                }
            }
        };

        /** Swap tuple1_t and tuple2_t element at the index if element is swappable. */
        template<usize type_index, typename tuple1_t, typename tuple2_t, bool = hud::is_swappable_v<hud::tuple_element_t<type_index, tuple1_t>, hud::tuple_element_t<type_index, tuple2_t>>>
        struct swap_element
        {
            template<typename>
            static constexpr bool EvaluateIfInstanciate = false;
            static_assert(EvaluateIfInstanciate<hud::false_type>, "type_t is not swappable");
        };

        template<usize type_index, typename... types_t, typename... u_types_t>
        struct swap_element<type_index, hud::compressed_tuple<types_t...>, hud::compressed_tuple<u_types_t...>, true>
        {
            constexpr void operator()(hud::compressed_tuple<types_t...> &first, hud::compressed_tuple<u_types_t...> &second) noexcept
            {
                hud::swap(hud::get<type_index>(first), hud::get<type_index>(second));
            }
        };

        /**
         * Recursively swap a compressed_tuple to another.
         * @tparam count Number of element to swap
         */
        template<usize count>
        struct tuple_swap
        {

            /**
             * Swap a 2 compressed_tuple elements.
             * @tparam types_t... List of types_t of the compressed_tuple first
             * @tparam u_types_t... List of types_t of the compressed_tuple second
             * @param first The first compressed_tuple to swap
             * @param second The second tulpe to swap
             */
            template<typename... types_t, typename... u_types_t>
            constexpr void operator()([[maybe_unused]] hud::compressed_tuple<types_t...> &first, [[maybe_unused]] hud::compressed_tuple<u_types_t...> &second) noexcept
            {
                static_assert(hud::is_same_size_v<hud::compressed_tuple<types_t...>, hud::compressed_tuple<u_types_t...>>, "Swapping tuples of different size is not supported");

                if constexpr (count > 0u)
                {
                    constexpr const usize index_to_swap = tuple_size_v<hud::compressed_tuple<types_t...>> - count;
                    swap_element<index_to_swap, hud::compressed_tuple<types_t...>, hud::compressed_tuple<u_types_t...>>()(first, second);
                    tuple_swap<count - 1u>()(first, second);
                }
            }
        };

        /**
         * Recursively compare a compressed_tuple to another.
         * @tparam count Number of element to compare
         */
        template<usize count>
        struct tuple_equals
        {
            /**
             * Compare a 2 compressed_tuple elements.
             * @tparam types_t... List of types_t of the compressed_tuple first
             * @tparam u_types_t... List of types_t of the compressed_tuple second
             * @param first The first compressed_tuple to compare
             * @param second The second tulpe to compare
             */
            template<typename... types_t, typename... u_types_t>
            [[nodiscard]] constexpr bool operator()([[maybe_unused]] const hud::compressed_tuple<types_t...> &first, [[maybe_unused]] const hud::compressed_tuple<u_types_t...> &second) noexcept
            {
                static_assert(hud::is_same_size_v<hud::compressed_tuple<types_t...>, hud::compressed_tuple<u_types_t...>>, "Comparing tuples of different size is not supported");
                if constexpr (count > 0u)
                {
                    constexpr const usize index_to_swap = tuple_size_v<hud::compressed_tuple<types_t...>> - count;
                    return hud::get<index_to_swap>(first) == hud::get<index_to_swap>(second) && tuple_equals<count - 1u>()(first, second);
                }
                else
                {
                    return true;
                }
            }
        };

        /**
         * Recursively compare a compressed_tuple to another with operator<.
         * @tparam count Number of element to compare
         */
        template<usize count>
        struct tuple_less
        {
            /**
             * Compare a 2 compressed_tuple elements with operator<.
             * @tparam types_t... List of types_t of the compressed_tuple first
             * @tparam u_types_t... List of types_t of the compressed_tuple second
             * @param first The first compressed_tuple to compare
             * @param second The second tulpe to compare
             */
            template<typename... types_t, typename... u_types_t>
            [[nodiscard]] constexpr bool operator()([[maybe_unused]] const hud::compressed_tuple<types_t...> &first, [[maybe_unused]] const hud::compressed_tuple<u_types_t...> &second) noexcept
            {
                static_assert(hud::is_same_size_v<hud::compressed_tuple<types_t...>, hud::compressed_tuple<u_types_t...>>, "Comparing tuples of different size is not supported");
                if constexpr (count > 0u)
                {
                    constexpr const usize index_to_swap = hud::tuple_size_v<hud::compressed_tuple<types_t...>> - count;
                    if (hud::get<index_to_swap>(first) < hud::get<index_to_swap>(second))
                    {
                        return true;
                    }
                    if (hud::get<index_to_swap>(second) < hud::get<index_to_swap>(first))
                    {
                        return false;
                    }
                    return tuple_less<count - 1u>()(first, second);
                }
                else
                {
                    return false;
                }
            }
        };

        template<typename... tuples_t>
        struct tuples_cat_impl
        {

            /**
             * Create a new compressed_tuple equivalent to all input tuples arguments inside a single compressed_tuple.
             * For exemple:
             *     If tuples_t... is compressed_tuple<u32, f32>, compressed_tuple<char, wchar, char16>, compressed_tuple<u64, f64>
             *     tuple_type is compressed_tuple<u32, f32, char, wchar, char16, u64 f64>
             */
            template<typename tuple_t, typename... rest_t>
            struct cat_tuples_arg
            {
                using tuple_type = tuple_t;
            };

            template<typename... args1_t, typename... args2_t, typename... rest_t>
            struct cat_tuples_arg<hud::compressed_tuple<args1_t...>, hud::compressed_tuple<args2_t...>, rest_t...>
            {
                using tuple_type = typename cat_tuples_arg<hud::compressed_tuple<args1_t..., args2_t...>, rest_t...>::tuple_type;
            };

            using return_type = typename cat_tuples_arg<tuples_t...>::tuple_type;

            /**
             * Create an hud::index_sequence of the tuples_t... list element index.
             * For exemple:
             *     If tuples_t... is compressed_tuple<u32, f32>, compressed_tuple<char, wchar, char16>, compressed_tuple<u64, f64>
             *     tuple_type is compressed_tuple<u32, f32, char, wchar, char16, u64 f64>
             *     element_index_seq is hud::index_sequence< 0,1, 0,1,2, 0,1>
             */
            template<typename element_indices_seq, typename...>
            struct cat_element_index
            {
                using element_index_seq = element_indices_seq;
            };

            template<usize... element_indices, typename... args_t, typename... rest_t>
            struct cat_element_index<hud::index_sequence<element_indices...>, hud::compressed_tuple<args_t...>, rest_t...>
            {
                using element_index_seq = typename cat_element_index<cat_integer_sequence_t<hud::index_sequence<element_indices...>, hud::make_index_sequence_for<args_t...>>, rest_t...>::element_index_seq;
            };

            using element_index_seq = typename cat_element_index<hud::make_index_sequence_for<>, tuples_t...>::element_index_seq;

            /**
             * Create a hud::index_sequence of the tuples_t... list index mask to match the element index hud::index_sequence.
             * For exemple:
             *     If tuples_t... is compressed_tuple<u32, f32>, compressed_tuple<char, wchar, char16>, compressed_tuple<u64, f64>
             *     tuple_type is compressed_tuple<u32, f32, char, wchar, char16, u64 f64>
             *     element_index_seq is hud::index_sequence< 0,1, 0,1,2, 0,1>
             *     mask_index_seq is hud::index_sequence< 0,0, 1,1,1, 2,2>
             */
            template<usize mask_index, typename type_t>
            static constexpr usize repeat_mask_index = mask_index;

            template<usize tuple_index, typename mask_seq, typename... rest_t>
            struct cat_mask_index
            {
                using mask_index_seq = mask_seq;
                static_assert(sizeof...(rest_t) == 0, "Unsupported tuple_cat arguments.");
            };

            template<usize tuple_index, usize... mask_indices, typename... args_t, typename... rest_t>
            struct cat_mask_index<tuple_index, hud::index_sequence<mask_indices...>, hud::compressed_tuple<args_t...>, rest_t...>
            {
                using mask_index_seq = typename cat_mask_index<tuple_index + 1u, cat_integer_sequence_t<hud::index_sequence<mask_indices...>, hud::index_sequence<repeat_mask_index<tuple_index, args_t>...>>, rest_t...>::mask_index_seq;
            };

            using mask_index_seq = typename cat_mask_index<0, hud::make_index_sequence_for<>, tuples_t...>::mask_index_seq;

            /**
             * Constructs a compressed_tuple that is a concatenation of all tuples in the given "compressed_tuple of tulpes" in the same order.
             * @tparam compressed_tuple The compressed_tuple type of tuples to concatenate
             * @tparam element_indices List element index
             * @tparam mask_indices List index mask to match the element index hud::index_sequence
             * @param hud::index_sequence of element_indices
             * @param hud::index_sequence of mask_indices
             * @param tulpe The compressed_tuple of tulpes to concatenate
             * @return The concatenated compressed_tuple
             */
            template<typename tuple_t, usize... element_indices, usize... mask_indices>
            static HD_FORCEINLINE constexpr return_type concatenate(hud::index_sequence<element_indices...>, hud::index_sequence<mask_indices...>, tuple_t &&compressed_tuple) noexcept
            {
                return return_type(hud::get<element_indices>(hud::get<mask_indices>(hud::forward<hud::compressed_tuple>(compressed_tuple)))...);
            }
        };

        template<>
        struct tuples_cat_impl<>
        {
            using return_type = hud::compressed_tuple<>;
            using element_index_seq = hud::index_sequence<>;
            using mask_index_seq = hud::index_sequence<>;

            /**
             * Constructs an empty compressed_tuple
             * @param tulpe The compressed_tuple of tulpes to concatenate
             * @return The concatenated compressed_tuple
             */
            template<typename EmptyTuple, usize... element_indices, usize... mask_indices>
            static HD_FORCEINLINE constexpr EmptyTuple concatenate(hud::index_sequence<element_indices...>, hud::index_sequence<mask_indices...>, EmptyTuple &&) noexcept
            {
                return EmptyTuple();
            }
        };

        template<typename... tuples_t>
        struct tuple_cat
        {

            using tuples_cat_impl_type = tuples_cat_impl<hud::decay_t<tuples_t>...>;
            using return_type = typename tuples_cat_impl_type::return_type;
            using element_index_seq = typename tuples_cat_impl_type::element_index_seq;
            using mask_index_seq = typename tuples_cat_impl_type::mask_index_seq;

            /**
             * Constructs a compressed_tuple that is a concatenation of all tuples in the given "compressed_tuple of tulpes" in the same order.
             * @tparam compressed_tuple The compressed_tuple type of tuples to concatenate
             * @param tulpe The compressed_tuple of tulpes to concatenate
             * @return The concatenated compressed_tuple
             */
            template<typename tuple_t>
            static HD_FORCEINLINE constexpr return_type concatenate(tuple_t &&compressed_tuple) noexcept
            {
                return tuples_cat_impl_type::concatenate(element_index_seq(), mask_index_seq(), hud::forward<tuple_t>(compressed_tuple));
            }
        };

        /**
         * Hashes all elements of a compressed_tuple and combines them into a single hash value.
         * This is a recursive template that processes one element per call.
         * @tparam count Number of elements remaining to hash.
         */
        template<usize count>
        struct tuple_hash
        {
            /**
             * Hashes the elements of the given compressed_tuple recursively, starting from the first unprocessed element.
             * @tparam Hasher Type of the hasher used.
             * @tparam types_t Types of the compressed_tuple elements.
             * @param hasher The hasher instance used to compute the hash.
             * @param t The compressed_tuple whose elements are to be hashed.
             * @return The final combined hash value.
             */
            template<typename Hasher, typename... types_t>
            [[nodiscard]] constexpr decltype(hud::declval<Hasher>().result()) operator()(Hasher &hasher, [[maybe_unused]] const hud::compressed_tuple<types_t...> &t) noexcept
            {
                if constexpr (count > 0u)
                {
                    constexpr const usize index_to_hash = tuple_size_v<hud::compressed_tuple<types_t...>> - count;
                    hasher.hash(hud::get<index_to_hash>(t));
                    return tuple_hash<count - 1u>()(hasher, t);
                }
                else
                {
                    return hasher.result();
                }
            }
        };

    } // namespace details::compressed_tuple

    /**
     * tuples_t are objects that pack elements of possibly different types together in a single object, just like pair objects do for pairs of elements, but generalized for any number of elements.
     * Conceptually, they are similar to plain old data structures (C-like structs) but instead of having named data members, its elements are accessed by their order in the compressed_tuple.
     * The selection of particular elements within a compressed_tuple is done at the template-instantiation level, and thus, it must be specified at compile-time, with helper functions.
     * The compressed_tuple class is closely related to the pair class: tuples_t can be constructed from pairs, and pairs can be treated as tuples for certain purposes.
     * @tparam types_t... List of types of the compressed_tuple
     */
    template<typename... types_t>
    class compressed_tuple
        : details::compressed_tuple::compressed_tuple_impl<hud::make_index_sequence_for<types_t...>, types_t...>
    {

    private:
        using super_type = details::compressed_tuple::compressed_tuple_impl<hud::make_index_sequence_for<types_t...>, types_t...>;

    public:
        /**
         * Default constructor.
         * This involves individually value-initializes all elements, if any.
         * compressed_tuple do not accept throwable default constructible components
         */
        explicit(hud::disjunction_v<hud::is_explicitly_default_constructible<types_t>...>) constexpr compressed_tuple(hud::tag_init_t) noexcept
        requires(hud::conjunction_v<hud::is_default_constructible<types_t>...>)
            : super_type(hud::tag_init)
        {
        }

        /**
         * Default constructor
         * This involves individually default constructs all components, with an initialization that depends on the constructor.
         * Constructs a compressed_tuple without value-initializing trivially default constructible elements.
         * compressed_tuple do not accept throwable default constructible components
         */
        explicit(hud::disjunction_v<hud::is_explicitly_default_constructible<types_t>...>) constexpr compressed_tuple() noexcept
        requires(hud::conjunction_v<hud::is_default_constructible<types_t>...>)
        {
        }

        /**
         * Initialization copy constructor.
         * This involves individually copy constructs all components, with an initialization that depends on the constructor.
         * compressed_tuple do not accept throwable copy constructible components.
         * @param args List of objects to copy construct into the compressed_tuple
         */
        explicit(hud::disjunction_v<hud::is_explicitly_copy_constructible<types_t>...>) constexpr compressed_tuple(const types_t &...args) noexcept
        requires(hud::conjunction_v<hud::bool_constant<sizeof...(types_t) >= 1>, hud::is_copy_constructible<types_t>...>)
            : super_type(args...)
        {
        }

        /**
         * Initialization move constructor.
         * This involves individually move constructs all components, with an initialization that depends on the constructor.
         * compressed_tuple do not accept throwable move constructible components.
         * @param args List of objects to move construct into the compressed_tuple
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<
                 hud::bool_constant<sizeof...(types_t) == sizeof...(u_types_t)>,
                 hud::bool_constant<sizeof...(types_t) >= 1>,
                 hud::is_constructible<types_t, u_types_t>...>)
        explicit(hud::disjunction_v<hud::is_explicitly_move_constructible<types_t, u_types_t>...>) constexpr compressed_tuple(u_types_t &&...args) noexcept
            : super_type(hud::forward<u_types_t>(args)...)
        {
        }

        /**
         * Piecewise constructor for `compressed_tuple` using tuples of arguments to construct each element.
         * This constructor forwards the elements of the tuples into the respective constructors.
         *
         * @param hud::tag_piecewise_construct_t Tag to indicate piecewise construction.
         * @param tuples Tuples containing arguments to forward to the constructor of each element.
         */
        template<typename... tuple_types>
        constexpr compressed_tuple(hud::tag_piecewise_construct_t, tuple_types &&...tuples) noexcept
            : super_type(hud::tag_piecewise_construct, hud::forward<tuple_types>(tuples)...)
        {
        }

        /**
         * Initialization copy constructor from a pair.
         * This involves individually copy constructs pair elements in compressed_tuple, with an initialization that depends on the constructor.
         * compressed_tuple do not accept throwable copy constructible components.
         * @tparam first_type type_t of the first component of pair
         * @tparam second_type type_t of the second component of pair
         * @param pair The pair to copy construct
         */
        template<typename first_type, typename second_type>
        requires(details::tuple::is_pair_copy_constructible_to_tuple_v<hud::pair<first_type, second_type>, compressed_tuple>)
        explicit(details::tuple::is_pair_explicitly_copy_constructible_to_tuple_v<hud::pair<first_type, second_type>, compressed_tuple>) constexpr compressed_tuple(const hud::pair<first_type, second_type> &pair) noexcept
            : compressed_tuple(pair.first, pair.second)
        {
        }

        /**
         * Initialization move constructor from a pair.
         * This involves individually move constructs pair elements in compressed_tuple, with an initialization that depends on the constructor.
         * compressed_tuple do not accept throwable move constructible components.
         * @tparam first_type type_t of the first component of pair
         * @tparam second_type type_t of the second component of pair
         * @param pair The pair to copy construct
         */
        template<typename first_type, typename second_type>
        requires(details::tuple::is_pair_move_constructible_to_tuple_v<hud::pair<first_type, second_type>, compressed_tuple>)
        explicit(details::tuple::is_pair_explicitly_move_constructible_to_tuple_v<hud::pair<first_type, second_type>, compressed_tuple>) constexpr compressed_tuple(hud::pair<first_type, second_type> &&pair) noexcept
            : compressed_tuple(hud::move(pair.first), hud::move(pair.second))
        {
        }

        /**
         * Copy constructor.
         * The object is initialized with the contents of the other compressed_tuple elements.
         * The corresponding members of other is passed to the copy constructor of each of its members.
         * compressed_tuple do not accept throwable copy constructible components.
         * @param other Another compressed_tuple object.
         */
        constexpr compressed_tuple(const compressed_tuple &other) noexcept
        requires(hud::conjunction_v<hud::is_copy_constructible<types_t>...>)
        = default;

        /**
         * Copy constructor.
         * The object is initialized with the contents of the other compressed_tuple object.
         * The corresponding members of other is passed to the copy constructor of each of its members.
         * compressed_tuple do not accept throwable copy constructible components.
         * @tparam u_types_t... List of types of the compressed_tuple to copy
         * @param other Another compressed_tuple object to copy
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<
                 hud::is_same_size<compressed_tuple, compressed_tuple<u_types_t...>>,
                 hud::is_copy_constructible<types_t, u_types_t>...>)
        explicit(hud::disjunction_v<hud::is_explicitly_copy_constructible<types_t, u_types_t>...>) constexpr compressed_tuple(const compressed_tuple<u_types_t...> &other) noexcept
            : compressed_tuple(other, hud::make_index_sequence_for<u_types_t...> {})
        {
        }

        /**
         * Move constructor.
         * The object is initialized with the contents of the other compressed_tuple elements.
         * The corresponding members of other is forward to the move constructor of each of its members.
         * compressed_tuple do not accept throwable move constructible components.
         * @param other Another compressed_tuple object to move
         */
        constexpr compressed_tuple(compressed_tuple &&other) noexcept = default;

        /**
         * Move constructor.
         * The object is initialized with the contents of the other compressed_tuple elements.
         * The corresponding members of other is forward to the move constructor of each of its members.
         * compressed_tuple do not accept throwable move constructible components.
         * @param other Another compressed_tuple object to move
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<
                 hud::is_same_size<compressed_tuple, compressed_tuple<u_types_t...>>,
                 hud::is_move_constructible<types_t, u_types_t>...>)
        explicit(hud::disjunction_v<hud::is_explicitly_move_constructible<types_t, u_types_t>...>) constexpr compressed_tuple(compressed_tuple<u_types_t...> &&other) noexcept
            : compressed_tuple(hud::forward<compressed_tuple<u_types_t...>>(other), hud::make_index_sequence_for<u_types_t...> {})
        {
        }

        /**
         * Assigns operator.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_tuple do not accept throwable copy assignable components.
         * @param other Another compressed_tuple to assign
         * @return *this
         */
        constexpr compressed_tuple &operator=(const compressed_tuple &other) noexcept
        requires(hud::conjunction_v<hud::is_copy_assignable<types_t>...>)
        {
            details::compressed_tuple::tuple_assign<sizeof...(types_t)>()(*this, other);
            return *this;
        }

        /**
         * Assigns operator.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_tuple do not accept throwable copy assignable components.
         * @param other Another compressed_tuple to assign
         * @return *this
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<hud::is_copy_assignable<types_t, u_types_t>...>)
        constexpr compressed_tuple &operator=(const compressed_tuple<u_types_t...> &other) noexcept
        {
            details::compressed_tuple::tuple_assign<sizeof...(types_t)>()(*this, other);
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_tuple object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * Tupel do not accept throwable move assignable components.
         * @param other Another compressed_tuple object.
         * @return *this
         */
        constexpr compressed_tuple &operator=(compressed_tuple &&other) noexcept
        requires(hud::conjunction_v<hud::is_move_assignable<types_t>...>)
        {
            details::compressed_tuple::tuple_assign<sizeof...(types_t)>()(*this, hud::move(other));
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_tuple object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * Tupel do not accept throwable move assignable components.
         * @param other Another compressed_tuple object.
         * @return *this
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<hud::is_move_assignable<types_t, u_types_t>...>)
        constexpr compressed_tuple &operator=(compressed_tuple<u_types_t...> &&other) noexcept
        {
            details::compressed_tuple::tuple_assign<sizeof...(types_t)>()(*this, hud::move(other));
            return *this;
        }

        template<usize element_index, typename... u_types_t>
        friend constexpr tuple_element_t<element_index, compressed_tuple<u_types_t...>> &get(compressed_tuple<u_types_t...> &compressed_tuple) noexcept;

        template<usize element_index, typename... u_types_t>
        friend constexpr const tuple_element_t<element_index, compressed_tuple<u_types_t...>> &get(const compressed_tuple<u_types_t...> &compressed_tuple) noexcept;

        template<usize element_index, typename... u_types_t>
        friend constexpr tuple_element_t<element_index, compressed_tuple<u_types_t...>> &&get(compressed_tuple<u_types_t...> &&compressed_tuple) noexcept;

        template<usize element_index, typename... u_types_t>
        friend constexpr const tuple_element_t<element_index, compressed_tuple<u_types_t...>> &&get(const compressed_tuple<u_types_t...> &&compressed_tuple) noexcept;

        template<usize idx_to_reach, typename... u_types_t>
        friend constexpr auto &&piecewise_get(hud::compressed_tuple<u_types_t...> &&compressed_tuple) noexcept;

    private:
        /**
         * Copy construct a compressed_tuple by unwrapping compressed_tuple element to copy and call initialisation constructor with unwrapped elements as parameter
         * @tparam tuple_t The type of compressed_tuple to copy
         * @tparam indices... The compressed_tuple element indices
         * @param compressed_tuple The compressed_tuple to copy
         * @param indices... The hud::index_sequence of indices
         */
        template<typename tuple_t, usize... indices>
        HD_FORCEINLINE constexpr explicit compressed_tuple(tuple_t &&compressed_tuple, hud::index_sequence<indices...>) noexcept
            : super_type(hud::get<indices>(hud::forward<tuple_t>(compressed_tuple))...)
        {
        }
    };

    /** Specialization for empty compressed_tuple. */
    template<>
    class compressed_tuple<>
    {
    };

    /**
     * is_same_size trait specialization for compressed_tuple.
     * @tparam types_t... List of types_t of the first compressed_tuple
     * @tparam u_types_t... List of types_t of the second compressed_tuple
     */
    template<typename... types_t, typename... u_types_t>
    struct is_same_size<compressed_tuple<types_t...>, compressed_tuple<u_types_t...>>
        : hud::bool_constant<sizeof...(types_t) == sizeof...(u_types_t)>
    {
    };

    /**
     * tuple_size specialization for compressed_tuple
     * @tparam types_t... List of types_t of the compressed_tuple
     */
    template<typename... types_t>
    struct tuple_size<compressed_tuple<types_t...>>
        : hud::integral_constant<usize, sizeof...(types_t)>
    {
    };

    /**
     * tuple_element specilization for compressed_tuple
     * @tparam index Index of the type to reach
     * @tparam type_t Current type
     * @tparam rest_t types_t after type_t in compressed_tuple
     */
    template<usize type_index>
    struct tuple_element<type_index, compressed_tuple<>>
    {
        // Out of bound index
        template<typename>
        static constexpr bool EVALUATE_IF_INSTANCIATE = false;
        static_assert(EVALUATE_IF_INSTANCIATE<hud::integral_constant<usize, type_index>>, "compressed_tuple index out of bounds");
    };

    template<typename type_t, typename... rest_t>
    struct tuple_element<0, compressed_tuple<type_t, rest_t...>>
    {
        // Reach given index, reach the index-th type_t
        using type = type_t;
    };

    template<usize type_index, typename type_t, typename... rest_t>
    struct tuple_element<type_index, compressed_tuple<type_t, rest_t...>>
        : tuple_element<type_index - 1, compressed_tuple<rest_t...>>
    {
        // Recursive definition, when reach 0 type_t is the one define above
    };

    /**
     * Returns an lvalue reference to the element at `idx_to_reach`.
     * @tparam idx_to_reach Index of the element to access.
     * @tparam types_t Parameter pack of the tuple’s types.
     * @param  t Tuple to access.
     * @return `T&` reference to the selected element.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &get(compressed_tuple<types_t...> &t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, compressed_tuple<types_t...>>;
        return static_cast<details::compressed_tuple::tuple_leaf_select<idx_to_reach, type_t>::type &>(t).element();
    }

    /**
     * Returns a const lvalue reference to the element at `idx_to_reach`.
     * @tparam idx_to_reach Index of the element to access.
     * @tparam types_t Parameter pack of the tuple’s types.
     * @param  t Tuple to access.
     * @return `const T&` reference to the selected element.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr const tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &get(const compressed_tuple<types_t...> &t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, compressed_tuple<types_t...>>;
        return static_cast<const details::compressed_tuple::tuple_leaf_select<idx_to_reach, type_t>::type &>(t).element();
    }

    /**
     * Returns an rvalue reference to the element at `idx_to_reach`.
     * @tparam idx_to_reach Index of the element to access.
     * @tparam types_t Parameter pack of the tuple’s types.
     * @param  t Tuple to access (rvalue).
     * @return `T&&` reference to the selected element, preserving cv‑qualifiers.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &&get(compressed_tuple<types_t...> &&t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, compressed_tuple<types_t...>>;
        return hud::forward<type_t>(static_cast<details::compressed_tuple::tuple_leaf_select<idx_to_reach, type_t>::type &&>(t).element());
    }

    /**
     * Returns a const rvalue reference to the element at `idx_to_reach`.
     * @tparam idx_to_reach Index of the element to access.
     * @tparam types_t Parameter pack of the tuple’s types.
     * @param  t Tuple to access (const rvalue).
     * @return `const T&&` reference to the selected element.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr const tuple_element_t<idx_to_reach, compressed_tuple<types_t...>> &&get(const compressed_tuple<types_t...> &&t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, compressed_tuple<types_t...>>;
        return hud::forward<const type_t>(static_cast<const details::compressed_tuple::tuple_leaf_select<idx_to_reach, type_t>::type &&>(t).element());
    }

    /**
     * Retrieves an rvalue reference to a compressed_tuple element for forwarding purposes.
     * This function extracts the element at the specified index from an rvalue compressed_tuple and
     * casts it explicitly to `T&&`, without preserving the value category or cv-qualifiers
     * of the stored element.
     * It is primarily intended for internal use in scenarios like piecewise construction,
     * where compressed_tuple elements are forwarded to construct other objects.
     * Warning: This function does **not** preserve reference or constness qualifiers.
     * Using it on tuples that contain reference types (`T&`, `const T&`) or non-movable types
     * may result in incorrect behavior or undefined reference binding.
     * For safe and category-preserving access to compressed_tuple elements, prefer using `get<Index>(std::move(compressed_tuple))` instead.
     * @tparam idx_to_reach The index of the element to extract.
     * @param t The rvalue compressed_tuple from which to extract the element.
     * @return An rvalue reference to the element at the specified index, cast as `T&&`
     *         without regard for its original value category.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] constexpr auto &&piecewise_get(hud::compressed_tuple<types_t...> &&t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, hud::compressed_tuple<types_t...>>;
        return static_cast<type_t &&>(static_cast<details::compressed_tuple::tuple_leaf_select<idx_to_reach, type_t>::type &>(t).content);
    }

    /**
     * swap specialization for compressed_tuple
     * @tparam types_t types_t of the compressed_tuple
     * @param first The first compressed_tuple to swap
     * @param second The second compressed_tuple to swap
     */
    template<typename... types_t, typename... u_types_t>
    static constexpr void swap(compressed_tuple<types_t...> &first, compressed_tuple<u_types_t...> &second) noexcept
    {
        details::compressed_tuple::tuple_swap<sizeof...(types_t)> {}(first, second);
    }

    /**
     * Checks if all elements of left compressed_tuple are equal to all elements at the same index of right compressed_tuple, that is, compares all elements by operator==.
     * @tparam types_t types_t of the left compressed_tuple
     * @tparam u_types_t types_t of the right compressed_tuple
     * @param first The first compressed_tuple to compare by operator==
     * @param second The second compressed_tuple to compareby operator==
     * @return true if all elements of left compressed_tuple are equal to all elements at the same index of right compressed_tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator==(const compressed_tuple<types_t...> &left, const compressed_tuple<u_types_t...> &right) noexcept
    {
        return details::compressed_tuple::tuple_equals<sizeof...(types_t)> {}(left, right);
    }

    /**
     * Checks if at least one elements of left compressed_tuple is not equals to the index corresponding elements of right compressed_tuple, that is, compares all elements by operator==.
     * @tparam types_t types_t of the left compressed_tuple
     * @tparam u_types_t types_t of the right compressed_tuple
     * @param first The first compressed_tuple to compare by operator==
     * @param second The second compressed_tuple to compareby operator==
     * @return true if at least one elements of left compressed_tuple is not equals to the index corresponding elements of right compressed_tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator!=(const compressed_tuple<types_t...> &left, const compressed_tuple<u_types_t...> &right) noexcept
    {
        return !(left == right);
    }

    /**
     * Checks if at least one elements of left compressed_tuple is not equals to the index corresponding elements of right compressed_tuple, that is, compares all elements by operator==.
     * @tparam types_t types_t of the compressed_tuple
     * @param first The first compressed_tuple to compare by operator==
     * @param second The second compressed_tuple to compareby operator==
     * @return true if at least one elements of left compressed_tuple is not equals to the index corresponding elements of right compressed_tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator<(const compressed_tuple<types_t...> &left, const compressed_tuple<u_types_t...> &right) noexcept
    {
        return details::compressed_tuple::tuple_less<sizeof...(types_t)> {}(left, right);
    }

    /**
     * Lexicographically compares all elements of left compressed_tuple are greater than all elements at the same index of right compressed_tuple, that is, compares all elements by operator<.
     * @tparam types_t types_t of the compressed_tuple
     * @param first The first compressed_tuple to compare by operator<
     * @param second The second compressed_tuple to compare by operator<
     * @return true if all elements of left compressed_tuple are greater than all elements at the same index of right compressed_tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator>(const compressed_tuple<types_t...> &left, const compressed_tuple<u_types_t...> &right) noexcept
    {
        return right < left;
    }

    /**
     * Lexicographically compares all elements of this compressed_tuple are less or equals all elements at the same index of another compressed_tuple, that is, compares all elements by operator<.
     * @tparam types_t types_t of the compressed_tuple
     * @param first The first compressed_tuple to compare by operator<
     * @param second The second compressed_tuple to compare by operator<
     * @return true all elements of this compressed_tuple is lexicographically less or equals the other elements at the same index, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator<=(const compressed_tuple<types_t...> &left, const compressed_tuple<u_types_t...> &right) noexcept
    {
        return !(left > right);
    }

    /**
     * Lexicographically compares all elements of left compressed_tuple are greater or equals than all elements at the same index of right compressed_tuple, that is, compares all elements by operator<.
     * @tparam types_t types_t of the compressed_tuple
     * @param first The first compressed_tuple to compare by operator<
     * @param second The second compressed_tuple to compare by operator<
     * @return true if all elements of left compressed_tuple are greater or equals than all elements at the same index of right compressed_tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator>=(const compressed_tuple<types_t...> &left, const compressed_tuple<u_types_t...> &right) noexcept
    {
        return !(left < right);
    }

    /**
     * Creates a compressed_tuple object, deducing the target type from the types of arguments.
     * @tparam types_t List of compressed_tuple types
     * @param args compressed_tuple constructor arguments list
     * @return compressed_tuple<types_t...> instance.
     */
    template<typename... types_t>
    [[nodiscard]] constexpr compressed_tuple<types_t...> make_tuple(types_t &&...args) noexcept
    {
        return compressed_tuple<types_t...>(hud::forward<types_t>(args)...);
    }

    /**
     * Constructs a compressed_tuple that is a concatenation of all tuples in args in the same order.
     * @tparam tuples_t All compressed_tuple types
     * @param args Zero or more compressed_tuple to concatenate
     * @return The concatenated compressed_tuple
     */
    template<typename... tuples_t>
    constexpr typename details::compressed_tuple::tuple_cat<tuples_t...>::return_type tuple_cat(tuples_t &&...args) noexcept
    {
        using tuple_cat_result = details::compressed_tuple::tuple_cat<tuples_t...>;
        return tuple_cat_result::concatenate(forward_as_tuple(hud::forward<tuples_t>(args)...));
    }

    template<typename... types_t>
    struct equal<hud::compressed_tuple<types_t...>>
    {
        template<typename... u_types_t>
        [[nodiscard]] constexpr bool operator()(const hud::compressed_tuple<types_t...> &lhs, const hud::compressed_tuple<u_types_t...> &rhs) const noexcept
        {
            return lhs == rhs;
        }
    };

    template<typename... types_t>
    struct hash_32<hud::compressed_tuple<types_t...>>
    {
        [[nodiscard]] constexpr u32 operator()(const hud::compressed_tuple<types_t...> &t) const noexcept
        {
            hasher_32 hasher;
            return details::compressed_tuple::tuple_hash<sizeof...(types_t)> {}(hasher, t);
        }
    };

    template<typename... types_t>
    struct hash_64<hud::compressed_tuple<types_t...>>
    {
        [[nodiscard]] constexpr u64 operator()(const hud::compressed_tuple<types_t...> &t) const noexcept
        {
            hasher_64 hasher;
            return details::compressed_tuple::tuple_hash<sizeof...(types_t)> {}(hasher, t);
        }
    };

} // namespace hud

namespace std
{
    template<typename... types_t>
    struct tuple_size<hud::compressed_tuple<types_t...>>
        : hud::tuple_size<typename hud::compressed_tuple<types_t...>>
    {
    };

    template<std::size_t index, typename... types_t>
    struct tuple_element<index, hud::compressed_tuple<types_t...>>
        : hud::tuple_element<index, hud::compressed_tuple<types_t...>>
    {
    };

} // namespace std
#endif // HD_INC_CORE_COMPRESSED_TUPLE_H