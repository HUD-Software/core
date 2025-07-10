#ifndef HD_INC_CORE_TUPLE_H
#define HD_INC_CORE_TUPLE_H
#include "../minimal.h"
#include "tuple_element.h"
#include "tuple_size.h"

#include "../templates/integer_sequence.h"
#include "../templates/swap.h"
#include "../templates/forward.h"

#include "../traits/is_same_size.h"
#include "../traits/enable_if.h"
#include "../traits/conjunction.h"
#include "../traits/disjunction.h"
#include "../traits/negation.h"
#include "../traits/is_default_constructible.h"
#include "../traits/is_implicitly_default_constructible.h"
#include "../traits/is_constructible.h"
#include "../traits/is_convertible.h"
#include "../traits/is_nothrow_default_constructible.h"
#include "../traits/is_nothrow_copy_constructible.h"
#include "../traits/is_nothrow_constructible.h"
#include "../traits/is_nothrow_copy_assignable.h"
#include "../traits/is_copy_constructible.h"
#include "../traits/is_copy_assignable.h"
#include "../traits/is_move_assignable.h"
#include "../traits/is_explicitly_constructible.h"
#include "../traits/is_explicitly_default_constructible.h"
#include "../traits/is_explicitly_copy_constructible.h"
#include "../traits/is_explicitly_move_constructible.h"
#include "../traits/is_not_same.h"
#include "../traits/is_swappable.h"
#include "../templates/tag_init.h"
#include "../templates/tag_piecewise_constrcut.h"

namespace hud
{
    template<typename type1_t, typename type2_t>
    struct pair;

    template<typename... types_t>
    class tuple;

    template<usize idx_to_reach, typename... types_t>
    constexpr tuple_element_t<idx_to_reach, tuple<types_t...>> &get(tuple<types_t...> &t) noexcept;
    template<usize idx_to_reach, typename... types_t>
    constexpr const tuple_element_t<idx_to_reach, tuple<types_t...>> &get(const tuple<types_t...> &t) noexcept;
    template<usize idx_to_reach, typename... types_t>
    constexpr tuple_element_t<idx_to_reach, tuple<types_t...>> &&get(tuple<types_t...> &&t) noexcept;
    template<usize idx_to_reach, typename... types_t>
    constexpr const tuple_element_t<idx_to_reach, tuple<types_t...>> &&get(const tuple<types_t...> &&t) noexcept;

    namespace details
    {

        /**
         * tuple_leaf is one tuple content associated with an index
         * @tparam leaf_index The index of the leaf
         * @tparam type_t The type of the content
         */
        template<usize leaf_index, typename type_t>
        struct tuple_leaf
        {

            /** Default constructor. Value-initialize content. */
            HD_FORCEINLINE constexpr tuple_leaf(hud::tag_init_t) noexcept
                : content()
            {
                static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t() default constructor is throwable. tuple is not designed to allow throwable default constructible components");
            }

            /** Default constructor. Do not initializes content. */
            HD_FORCEINLINE constexpr tuple_leaf() noexcept
            {
                static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t() default constructor is throwable. tuple is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization copy constructor.
             * @param arg Object to copy construct into the tuple leaf
             */
            HD_FORCEINLINE constexpr tuple_leaf(const type_t &arg) noexcept
                : content(arg)
            {
                static_assert(hud::is_nothrow_copy_constructible_v<type_t>, "type_t(const type_t&) copy constructor is throwable. tuple is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * @tparam UType type_t of arg parameter
             * @param arg Object to move construct into the tuple leaf
             */
            template<typename UType>
            requires(hud::is_constructible_v<type_t, UType>)
            HD_FORCEINLINE constexpr tuple_leaf(UType &&arg) noexcept
                : content(hud::forward<UType>(arg))
            {
                static_assert(hud::is_nothrow_move_constructible_v<type_t, UType>, "type_t(UType&&) move constructor is throwable. tuple is not designed to allow throwable move constructible components");
            }

            /**
             * Piecewise constructor forwarding a tuple of arguments.
             * @tparam Args Types of the elements in the tuple used for construction.
             * @param tuple Tuple of arguments to forward to the constructor of type_t.
             */
            template<typename... Args>
            constexpr tuple_leaf(hud::tag_piecewise_construct_t, hud::tuple<Args...> &&tuple) noexcept
                : tuple_leaf(hud::forward<hud::tuple<Args...>>(tuple), hud::make_index_sequence_for<Args...> {})
            {
            }

            /**
             * Piecewise constructor that unpacks a tuple into individual arguments.
             *
             * @tparam Args Types of the elements in the tuple used for construction.
             * @tparam indices Index sequence used to extract each element from the tuple.
             * @param tuple Tuple whose elements are forwarded to the constructor of type_t.
             */
            template<typename... Args, usize... indices>
            constexpr tuple_leaf(hud::tuple<Args...> &&tuple, hud::index_sequence<indices...>) noexcept
                : content(hud::forward<Args>(hud::get<indices>(tuple))...)
            {
                static_assert(hud::is_nothrow_constructible_v<type_t, Args...>, "type_t(Args&&...) constructor is throwable. pair is not designed to allow throwable constructible components");
            }

            /**
             * Assigns operator.
             * @tparam UType type_t of other tuple_leaf parameter
             * @param other Another tuple leaf
             * @return *this
             */
            template<typename UType>
            HD_FORCEINLINE constexpr tuple_leaf &operator=(UType &&arg) noexcept
            {
                static_assert(hud::is_nothrow_assignable_v<type_t &, UType &&>, "type_t& type_t::operator=(const UType&) is throwable. tuple is not designed to allow throwable copy assignable components");
                content = hud::forward<UType>(arg);
                return *this;
            }

            /** The element */
            type_t content;
        };

        /**
         * tuple_impl is the implementation of the tuple with an sequence of index and a list of types
         * indices is used to access tuple_leaf that contains the type_t where types_t[indices]... relation is respected.
         * @tparam index_seq_t hud::index_sequence of types_t...
         * @tparam types_t... List of types_t of the tuple
         */
        template<typename index_seq_t, typename... types_t>
        struct tuple_impl;

        template<usize... indices, typename... types_t>
        struct tuple_impl<index_sequence<indices...>, types_t...>
            : tuple_leaf<indices, types_t>...
        {

            /**
             * Default constructor that calls all tuple leafs default constructors.
             * Value-initializes all elements, if any.
             */
            constexpr tuple_impl(tag_init_t) noexcept
                : tuple_leaf<indices, types_t>(hud::tag_init)...
            {
            }

            /**
             * Default constructor that calls all tuple leafs default constructors.
             * Do not initializes elements, if any.
             */
            HD_FORCEINLINE constexpr tuple_impl() noexcept
                : tuple_leaf<indices, types_t>()...
            {
            }

            /**
             * Initialization copy constructor that calls all tuple leafs initialization copy constructors.
             * @param args List of objects to copy into the tuple
             */
            HD_FORCEINLINE constexpr tuple_impl(const types_t &...args) noexcept
                : tuple_leaf<indices, types_t>(args)...
            {
            }

            /**
             * Initialization move constructor that calls all tuple leafs initialization move constructors.
             * @param args List of objects to move into the tuple
             */
            template<typename... u_types_t>
            HD_FORCEINLINE constexpr tuple_impl(u_types_t &&...args) noexcept
                : tuple_leaf<indices, types_t>(hud::forward<u_types_t>(args))...
            {
            }

            /**
             * Piecewise constructor for `tuple` using tuples of arguments to construct each tuple leafs.
             * @param hud::tag_piecewise_construct_t Tag to indicate piecewise construction.
             * @param tuples Tuples containing arguments to forward to the constructor of each leafs.
             */
            template<typename... tuple_types>
            constexpr tuple_impl(hud::tag_piecewise_construct_t, tuple_types &&...tuples) noexcept
                : tuple_leaf<indices, types_t>(hud::tag_piecewise_construct, hud::forward<tuple_types>(tuples))...
            {
            }

            /** Copy constructor */
            constexpr tuple_impl(const tuple_impl &) = default;

            /** Move constructor */
            constexpr tuple_impl(tuple_impl &&) = default;
        };

        /**
         * @brief Check if a tuple<T0,T1> is copy constructible from pair<first_type,second_type>.
         * @tparam pair_t The hud::pair
         * @tparam tuple_t  The hud::tuple
         */
        template<typename pair_t, typename tuple_t>
        struct is_pair_copy_constructible_to_tuple
            : hud::false_type
        {
        };

        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_copy_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::tuple<tuple_type0_t, tuple_type1_t>>
            : hud::conjunction<
                  hud::is_copy_constructible<tuple_type0_t, pair_type0_t>,
                  hud::is_copy_constructible<tuple_type1_t, pair_type1_t>>
        {
        };

        template<typename pair_t, typename tuple_t>
        inline constexpr bool is_pair_copy_constructible_to_tuple_v = is_pair_copy_constructible_to_tuple<pair_t, tuple_t>::value;

        /**
         * Check if a tuple<T0,T1> is explicitly copy constructible from pair<first_type,second_type>.
         * In other words, if T0(const first_type{}) is explicit and T1(const second_type{}) is explicit
         */
        template<typename pair_t, typename tuple_t>
        struct is_pair_explicitly_copy_constructible_to_tuple
            : hud::false_type
        {
        };

        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_explicitly_copy_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::tuple<tuple_type0_t, tuple_type1_t>>
            : hud::disjunction<hud::is_explicitly_copy_constructible<tuple_type0_t, pair_type0_t>, hud::is_explicitly_copy_constructible<tuple_type1_t, pair_type1_t>>
        {
        };

        template<typename pair_t, typename tuple_t>
        inline constexpr bool is_pair_explicitly_copy_constructible_to_tuple_v = is_pair_explicitly_copy_constructible_to_tuple<pair_t, tuple_t>::value;

        /**
         * @brief Check if a tuple<T0,T1> is move constructible from pair<first_type,second_type>.
         * @tparam pair_t The hud::pair
         * @tparam tuple_t  The hud::tuple
         */
        template<typename pair_t, typename tuple_t>
        struct is_pair_move_constructible_to_tuple
            : hud::false_type
        {
        };

        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_move_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::tuple<tuple_type0_t, tuple_type1_t>>
            : hud::conjunction<
                  hud::is_move_constructible<tuple_type0_t, pair_type0_t>,
                  hud::is_move_constructible<tuple_type1_t, pair_type1_t>>
        {
        };

        template<typename pair_t, typename tuple_t>
        inline constexpr bool is_pair_move_constructible_to_tuple_v = is_pair_move_constructible_to_tuple<pair_t, tuple_t>::value;

        /**
         * Check if a tuple<T0,T1> is explicitly move constructible from pair<first_type,second_type>.
         * In other words, if T0(first_type{}) is explicit and T1(second_type{}) is explicit
         */
        template<typename pair_t, typename tuple_t>
        struct is_pair_explicitly_move_constructible_to_tuple
            : hud::false_type
        {
        };

        template<typename pair_type0_t, typename pair_type1_t, typename tuple_type0_t, typename tuple_type1_t>
        struct is_pair_explicitly_move_constructible_to_tuple<hud::pair<pair_type0_t, pair_type1_t>, hud::tuple<tuple_type0_t, tuple_type1_t>>
            : hud::disjunction<hud::is_explicitly_move_constructible<tuple_type0_t, pair_type0_t>, hud::is_explicitly_move_constructible<tuple_type1_t, pair_type1_t>>
        {
        };

        template<typename pair_t, typename tuple_t>
        inline constexpr bool is_pair_explicitly_move_constructible_to_tuple_v = is_pair_explicitly_move_constructible_to_tuple<pair_t, tuple_t>::value;

        /**
         * Recursively assign a tuple to another.
         * @tparam count Number of element to assign
         */
        template<usize count>
        struct tuple_assign
        {

            /**
             * Assign a 2 tuple elements.
             * @tparam types_t... List of types_t of the tuple to
             * @tparam u_types_t... List of types_t of the tuple from
             * @param to The assigned tuple
             * @param from The tuple to assign
             */
            template<typename... types_t, typename... u_types_t>
            constexpr void operator()([[maybe_unused]] tuple<types_t...> &to, [[maybe_unused]] const tuple<u_types_t...> &from) noexcept
            {
                static_assert(hud::is_same_size_v<tuple<types_t...>, tuple<u_types_t...>>, "Assigning tuples of different size is not supported");
                if constexpr (count > 0u)
                {
                    constexpr const usize idx = tuple_size_v<tuple<types_t...>> - count;
                    hud::get<idx>(to) = hud::get<idx>(from);
                    tuple_assign<count - 1u>()(to, from);
                }
            }

            /**
             * Assign a 2 tuple elements.
             * @tparam types_t... List of types_t of the tuple to
             * @tparam u_types_t... List of types_t of the tuple from
             * @param to The assigned tuple
             * @param from The tuple to assign
             */
            template<typename... types_t, typename... u_types_t>
            constexpr void operator()([[maybe_unused]] tuple<types_t...> &to, [[maybe_unused]] tuple<u_types_t...> &&from) noexcept
            {
                static_assert(hud::is_same_size_v<tuple<types_t...>, tuple<u_types_t...>>, "Assigning tuples of different size is not supported");
                if constexpr (count > 0)
                {
                    constexpr const usize idx = tuple_size_v<tuple<types_t...>> - count;
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
        struct swap_element<type_index, tuple<types_t...>, tuple<u_types_t...>, true>
        {
            constexpr void operator()(tuple<types_t...> &first, tuple<u_types_t...> &second) noexcept
            {
                hud::swap(hud::get<type_index>(first), hud::get<type_index>(second));
            }
        };

        /**
         * Recursively swap a tuple to another.
         * @tparam count Number of element to swap
         */
        template<usize count>
        struct tuple_swap
        {

            /**
             * Swap a 2 tuple elements.
             * @tparam types_t... List of types_t of the tuple first
             * @tparam u_types_t... List of types_t of the tuple second
             * @param first The first tuple to swap
             * @param second The second tulpe to swap
             */
            template<typename... types_t, typename... u_types_t>
            constexpr void operator()([[maybe_unused]] tuple<types_t...> &first, [[maybe_unused]] tuple<u_types_t...> &second) noexcept
            {
                static_assert(hud::is_same_size_v<tuple<types_t...>, tuple<u_types_t...>>, "Swapping tuples of different size is not supported");

                if constexpr (count > 0u)
                {
                    constexpr const usize index_to_swap = tuple_size_v<tuple<types_t...>> - count;
                    swap_element<index_to_swap, tuple<types_t...>, tuple<u_types_t...>>()(first, second);
                    tuple_swap<count - 1u>()(first, second);
                }
            }
        };

        /**
         * Recursively compare a tuple to another.
         * @tparam count Number of element to compare
         */
        template<usize count>
        struct tuple_equals
        {
            /**
             * Compare a 2 tuple elements.
             * @tparam types_t... List of types_t of the tuple first
             * @tparam u_types_t... List of types_t of the tuple second
             * @param first The first tuple to compare
             * @param second The second tulpe to compare
             */
            template<typename... types_t, typename... u_types_t>
            [[nodiscard]] constexpr bool operator()([[maybe_unused]] const tuple<types_t...> &first, [[maybe_unused]] const tuple<u_types_t...> &second) noexcept
            {
                static_assert(hud::is_same_size_v<tuple<types_t...>, tuple<u_types_t...>>, "Comparing tuples of different size is not supported");
                if constexpr (count > 0u)
                {
                    constexpr const usize index_to_swap = tuple_size_v<tuple<types_t...>> - count;
                    return hud::get<index_to_swap>(first) == hud::get<index_to_swap>(second) && tuple_equals<count - 1u>()(first, second);
                }
                else
                {
                    return true;
                }
            }
        };

        /**
         * Recursively compare a tuple to another with operator<.
         * @tparam count Number of element to compare
         */
        template<usize count>
        struct tuple_less
        {
            /**
             * Compare a 2 tuple elements with operator<.
             * @tparam types_t... List of types_t of the tuple first
             * @tparam u_types_t... List of types_t of the tuple second
             * @param first The first tuple to compare
             * @param second The second tulpe to compare
             */
            template<typename... types_t, typename... u_types_t>
            [[nodiscard]] constexpr bool operator()([[maybe_unused]] const tuple<types_t...> &first, [[maybe_unused]] const tuple<u_types_t...> &second) noexcept
            {
                static_assert(hud::is_same_size_v<tuple<types_t...>, tuple<u_types_t...>>, "Comparing tuples of different size is not supported");
                if constexpr (count > 0u)
                {
                    constexpr const usize index_to_swap = hud::tuple_size_v<tuple<types_t...>> - count;
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
             * Create a new tuple equivalent to all input tuples arguments inside a single tuple.
             * For exemple:
             *     If tuples_t... is tuple<u32, f32>, tuple<char, wchar, char16>, tuple<u64, f64>
             *     tuple_type is tuple<u32, f32, char, wchar, char16, u64 f64>
             */
            template<typename tuple_t, typename... rest_t>
            struct cat_tuples_arg
            {
                using tuple_type = tuple_t;
            };

            template<typename... args1_t, typename... args2_t, typename... rest_t>
            struct cat_tuples_arg<tuple<args1_t...>, tuple<args2_t...>, rest_t...>
            {
                using tuple_type = typename cat_tuples_arg<tuple<args1_t..., args2_t...>, rest_t...>::tuple_type;
            };

            using return_type = typename cat_tuples_arg<tuples_t...>::tuple_type;

            /**
             * Create an hud::index_sequence of the tuples_t... list element index.
             * For exemple:
             *     If tuples_t... is tuple<u32, f32>, tuple<char, wchar, char16>, tuple<u64, f64>
             *     tuple_type is tuple<u32, f32, char, wchar, char16, u64 f64>
             *     element_index_seq is hud::index_sequence< 0,1, 0,1,2, 0,1>
             */
            template<typename element_indices_seq, typename...>
            struct cat_element_index
            {
                using element_index_seq = element_indices_seq;
            };

            template<usize... element_indices, typename... args_t, typename... rest_t>
            struct cat_element_index<hud::index_sequence<element_indices...>, tuple<args_t...>, rest_t...>
            {
                using element_index_seq = typename cat_element_index<cat_integer_sequence_t<hud::index_sequence<element_indices...>, hud::make_index_sequence_for<args_t...>>, rest_t...>::element_index_seq;
            };

            using element_index_seq = typename cat_element_index<hud::make_index_sequence_for<>, tuples_t...>::element_index_seq;

            /**
             * Create a hud::index_sequence of the tuples_t... list index mask to match the element index hud::index_sequence.
             * For exemple:
             *     If tuples_t... is tuple<u32, f32>, tuple<char, wchar, char16>, tuple<u64, f64>
             *     tuple_type is tuple<u32, f32, char, wchar, char16, u64 f64>
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
            struct cat_mask_index<tuple_index, hud::index_sequence<mask_indices...>, tuple<args_t...>, rest_t...>
            {
                using mask_index_seq = typename cat_mask_index<tuple_index + 1u, cat_integer_sequence_t<hud::index_sequence<mask_indices...>, hud::index_sequence<repeat_mask_index<tuple_index, args_t>...>>, rest_t...>::mask_index_seq;
            };

            using mask_index_seq = typename cat_mask_index<0, hud::make_index_sequence_for<>, tuples_t...>::mask_index_seq;

            /**
             * Constructs a tuple that is a concatenation of all tuples in the given "tuple of tulpes" in the same order.
             * @tparam tuple The tuple type of tuples to concatenate
             * @tparam element_indices List element index
             * @tparam mask_indices List index mask to match the element index hud::index_sequence
             * @param hud::index_sequence of element_indices
             * @param hud::index_sequence of mask_indices
             * @param tulpe The tuple of tulpes to concatenate
             * @return The concatenated tuple
             */
            template<typename tuple_t, usize... element_indices, usize... mask_indices>
            static HD_FORCEINLINE constexpr return_type concatenate(hud::index_sequence<element_indices...>, hud::index_sequence<mask_indices...>, tuple_t &&tuple) noexcept
            {
                return return_type(hud::get<element_indices>(hud::get<mask_indices>(hud::forward<tuple>(tuple)))...);
            }
        };

        template<>
        struct tuples_cat_impl<>
        {
            using return_type = tuple<>;
            using element_index_seq = hud::index_sequence<>;
            using mask_index_seq = hud::index_sequence<>;

            /**
             * Constructs an empty tuple
             * @param tulpe The tuple of tulpes to concatenate
             * @return The concatenated tuple
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
             * Constructs a tuple that is a concatenation of all tuples in the given "tuple of tulpes" in the same order.
             * @tparam tuple The tuple type of tuples to concatenate
             * @param tulpe The tuple of tulpes to concatenate
             * @return The concatenated tuple
             */
            template<typename tuple_t>
            static HD_FORCEINLINE constexpr return_type concatenate(tuple_t &&tuple) noexcept
            {
                return tuples_cat_impl_type::concatenate(element_index_seq(), mask_index_seq(), hud::forward<tuple_t>(tuple));
            }
        };
    } // namespace details

    /**
     * tuples_t are objects that pack elements of possibly different types together in a single object, just like pair objects do for pairs of elements, but generalized for any number of elements.
     * Conceptually, they are similar to plain old data structures (C-like structs) but instead of having named data members, its elements are accessed by their order in the tuple.
     * The selection of particular elements within a tuple is done at the template-instantiation level, and thus, it must be specified at compile-time, with helper functions.
     * The tuple class is closely related to the pair class: tuples_t can be constructed from pairs, and pairs can be treated as tuples for certain purposes.
     * @tparam types_t... List of types of the tuple
     */
    template<typename... types_t>
    class tuple
        : details::tuple_impl<hud::make_index_sequence_for<types_t...>, types_t...>
    {

    private:
        using super_type = details::tuple_impl<hud::make_index_sequence_for<types_t...>, types_t...>;

    public:
        /**
         * Default constructor.
         * This involves individually value-initializes all elements, if any.
         * tuple do not accept throwable default constructible components
         */
        explicit(hud::disjunction_v<hud::is_explicitly_default_constructible<types_t>...>) constexpr tuple(hud::tag_init_t) noexcept
        requires(hud::conjunction_v<hud::is_default_constructible<types_t>...>)
            : super_type(hud::tag_init)
        {
        }

        /**
         * Default constructor
         * This involves individually default constructs all components, with an initialization that depends on the constructor.
         * Constructs a tuple without value-initializing trivially default constructible elements.
         * tuple do not accept throwable default constructible components
         */
        explicit(hud::disjunction_v<hud::is_explicitly_default_constructible<types_t>...>) constexpr tuple() noexcept
        requires(hud::conjunction_v<hud::is_default_constructible<types_t>...>)
        {
        }

        /**
         * Initialization copy constructor.
         * This involves individually copy constructs all components, with an initialization that depends on the constructor.
         * tuple do not accept throwable copy constructible components.
         * @param args List of objects to copy construct into the tuple
         */
        explicit(hud::disjunction_v<hud::is_explicitly_copy_constructible<types_t>...>) constexpr tuple(const types_t &...args) noexcept
        requires(hud::conjunction_v<hud::bool_constant<sizeof...(types_t) >= 1>, hud::is_copy_constructible<types_t>...>)
            : super_type(args...)
        {
        }

        /**
         * Initialization move constructor.
         * This involves individually move constructs all components, with an initialization that depends on the constructor.
         * tuple do not accept throwable move constructible components.
         * @param args List of objects to move construct into the tuple
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<
                 hud::bool_constant<sizeof...(types_t) == sizeof...(u_types_t)>,
                 hud::bool_constant<sizeof...(types_t) >= 1>,
                 hud::is_constructible<types_t, u_types_t>...>)
        explicit(hud::disjunction_v<hud::is_explicitly_move_constructible<types_t, u_types_t>...>) constexpr tuple(u_types_t &&...args) noexcept
            : super_type(hud::forward<u_types_t>(args)...)
        {
        }

        /**
         * Piecewise constructor for `tuple` using tuples of arguments to construct each element.
         * This constructor forwards the elements of the tuples into the respective constructors.
         *
         * @param hud::tag_piecewise_construct_t Tag to indicate piecewise construction.
         * @param tuples Tuples containing arguments to forward to the constructor of each element.
         */
        template<typename... tuple_types>
        constexpr tuple(hud::tag_piecewise_construct_t, tuple_types &&...tuples) noexcept
            : super_type(hud::tag_piecewise_construct, hud::forward<tuple_types>(tuples)...)
        {
        }

        /**
         * Initialization copy constructor from a pair.
         * This involves individually copy constructs pair elements in tuple, with an initialization that depends on the constructor.
         * tuple do not accept throwable copy constructible components.
         * @tparam first_type type_t of the first component of pair
         * @tparam second_type type_t of the second component of pair
         * @param pair The pair to copy construct
         */
        template<typename first_type, typename second_type>
        requires(details::is_pair_copy_constructible_to_tuple_v<hud::pair<first_type, second_type>, tuple>)
        explicit(details::is_pair_explicitly_copy_constructible_to_tuple_v<hud::pair<first_type, second_type>, tuple>) constexpr tuple(const hud::pair<first_type, second_type> &pair) noexcept
            : tuple(pair.first, pair.second)
        {
        }

        /**
         * Initialization move constructor from a pair.
         * This involves individually move constructs pair elements in tuple, with an initialization that depends on the constructor.
         * tuple do not accept throwable move constructible components.
         * @tparam first_type type_t of the first component of pair
         * @tparam second_type type_t of the second component of pair
         * @param pair The pair to copy construct
         */
        template<typename first_type, typename second_type>
        requires(details::is_pair_move_constructible_to_tuple_v<hud::pair<first_type, second_type>, tuple>)
        explicit(details::is_pair_explicitly_move_constructible_to_tuple_v<hud::pair<first_type, second_type>, tuple>) constexpr tuple(hud::pair<first_type, second_type> &&pair) noexcept
            : tuple(hud::move(pair.first), hud::move(pair.second))
        {
        }

        /**
         * Copy constructor.
         * The object is initialized with the contents of the other tuple elements.
         * The corresponding members of other is passed to the copy constructor of each of its members.
         * tuple do not accept throwable copy constructible components.
         * @param other Another tuple object.
         */
        constexpr tuple(const tuple &other) noexcept
        requires(hud::conjunction_v<hud::is_copy_constructible<types_t>...>)
        = default;

        /**
         * Copy constructor.
         * The object is initialized with the contents of the other tuple object.
         * The corresponding members of other is passed to the copy constructor of each of its members.
         * tuple do not accept throwable copy constructible components.
         * @tparam u_types_t... List of types of the tuple to copy
         * @param other Another tuple object to copy
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<
                 hud::is_same_size<tuple, tuple<u_types_t...>>,
                 hud::is_copy_constructible<types_t, u_types_t>...>)
        explicit(hud::disjunction_v<hud::is_explicitly_copy_constructible<types_t, u_types_t>...>) constexpr tuple(const tuple<u_types_t...> &other) noexcept
            : tuple(other, hud::make_index_sequence_for<u_types_t...> {})
        {
        }

        /**
         * Move constructor.
         * The object is initialized with the contents of the other tuple elements.
         * The corresponding members of other is forward to the move constructor of each of its members.
         * tuple do not accept throwable move constructible components.
         * @param other Another tuple object to move
         */
        constexpr tuple(tuple &&other) noexcept = default;

        /**
         * Move constructor.
         * The object is initialized with the contents of the other tuple elements.
         * The corresponding members of other is forward to the move constructor of each of its members.
         * tuple do not accept throwable move constructible components.
         * @param other Another tuple object to move
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<
                 hud::is_same_size<tuple, tuple<u_types_t...>>,
                 hud::is_move_constructible<types_t, u_types_t>...>)
        explicit(hud::disjunction_v<hud::is_explicitly_move_constructible<types_t, u_types_t>...>) constexpr tuple(tuple<u_types_t...> &&other) noexcept
            : tuple(hud::forward<tuple<u_types_t...>>(other), hud::make_index_sequence_for<u_types_t...> {})
        {
        }

        /**
         * Assigns operator.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * tuple do not accept throwable copy assignable components.
         * @param other Another tuple to assign
         * @return *this
         */
        constexpr tuple &operator=(const tuple &other) noexcept
        requires(hud::conjunction_v<hud::is_copy_assignable<types_t>...>)
        {
            details::tuple_assign<sizeof...(types_t)>()(*this, other);
            return *this;
        }

        /**
         * Assigns operator.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * tuple do not accept throwable copy assignable components.
         * @param other Another tuple to assign
         * @return *this
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<hud::is_copy_assignable<types_t, u_types_t>...>)
        constexpr tuple &operator=(const tuple<u_types_t...> &other) noexcept
        {
            details::tuple_assign<sizeof...(types_t)>()(*this, other);
            return *this;
        }

        /**
         * Assigns other as the new content for the tuple object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * Tupel do not accept throwable move assignable components.
         * @param other Another tuple object.
         * @return *this
         */
        constexpr tuple &operator=(tuple &&other) noexcept
        requires(hud::conjunction_v<hud::is_move_assignable<types_t>...>)
        {
            details::tuple_assign<sizeof...(types_t)>()(*this, hud::move(other));
            return *this;
        }

        /**
         * Assigns other as the new content for the tuple object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * Tupel do not accept throwable move assignable components.
         * @param other Another tuple object.
         * @return *this
         */
        template<typename... u_types_t>
        requires(hud::conjunction_v<hud::is_move_assignable<types_t, u_types_t>...>)
        constexpr tuple &operator=(tuple<u_types_t...> &&other) noexcept
        {
            details::tuple_assign<sizeof...(types_t)>()(*this, hud::move(other));
            return *this;
        }

        template<usize element_index, typename... u_types_t>
        friend constexpr tuple_element_t<element_index, tuple<u_types_t...>> &get(tuple<u_types_t...> &tuple) noexcept;

        template<usize element_index, typename... u_types_t>
        friend constexpr const tuple_element_t<element_index, tuple<u_types_t...>> &get(const tuple<u_types_t...> &tuple) noexcept;

        template<usize element_index, typename... u_types_t>
        friend constexpr tuple_element_t<element_index, tuple<u_types_t...>> &&get(tuple<u_types_t...> &&tuple) noexcept;

        template<usize element_index, typename... u_types_t>
        friend constexpr const tuple_element_t<element_index, tuple<u_types_t...>> &&get(const tuple<u_types_t...> &&tuple) noexcept;

        template<usize idx_to_reach, typename... types_t>
        friend constexpr auto &&piecewise_get(hud::tuple<types_t...> &&tuple) noexcept;

    private:
        /**
         * Copy construct a tuple by unwrapping tuple element to copy and call initialisation constructor with unwrapped elements as parameter
         * @tparam tuple_t The type of tuple to copy
         * @tparam indices... The tuple element indices
         * @param tuple The tuple to copy
         * @param indices... The hud::index_sequence of indices
         */
        template<typename tuple_t, usize... indices>
        HD_FORCEINLINE constexpr explicit tuple(tuple_t &&tuple, hud::index_sequence<indices...>) noexcept
            : super_type(hud::get<indices>(hud::forward<tuple_t>(tuple))...)
        {
        }
    };

    /** Specialization for empty tuple. */
    template<>
    class tuple<>
    {
    };

    /**
     * is_same_size trait specialization for tuple.
     * @tparam types_t... List of types_t of the first tuple
     * @tparam u_types_t... List of types_t of the second tuple
     */
    template<typename... types_t, typename... u_types_t>
    struct is_same_size<tuple<types_t...>, tuple<u_types_t...>>
        : hud::bool_constant<sizeof...(types_t) == sizeof...(u_types_t)>
    {
    };

    /**
     * tuple_size specialization for tuple
     * @tparam types_t... List of types_t of the tuple
     */
    template<typename... types_t>
    struct tuple_size<tuple<types_t...>>
        : hud::integral_constant<usize, sizeof...(types_t)>
    {
    };

    /**
     * tuple_element specilization for tuple
     * @tparam index Index of the type to reach
     * @tparam type_t Current type
     * @tparam rest_t types_t after type_t in tuple
     */
    template<usize type_index>
    struct tuple_element<type_index, tuple<>>
    {
        // Out of bound index
        template<typename>
        static constexpr bool EVALUATE_IF_INSTANCIATE = false;
        static_assert(EVALUATE_IF_INSTANCIATE<hud::integral_constant<usize, type_index>>, "tuple index out of bounds");
    };

    template<typename type_t, typename... rest_t>
    struct tuple_element<0, tuple<type_t, rest_t...>>
    {
        // Reach given index, reach the index-th type_t
        using type = type_t;
    };

    template<usize type_index, typename type_t, typename... rest_t>
    struct tuple_element<type_index, tuple<type_t, rest_t...>>
        : tuple_element<type_index - 1, tuple<rest_t...>>
    {
        // Recursive definition, when reach 0 type_t is the one define above
    };

    /**
     * Retrieves a lvalue reference to the member of a tuple at an index
     * @tparam idx_to_reach The index to reach
     * @tparam types_t... types_t of the tuple
     * @param tuple The tuple to access
     * @return LValue reference to the member mFirst if index is 0, mSecond if index is 1.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr tuple_element_t<idx_to_reach, tuple<types_t...>> &get(tuple<types_t...> &t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, tuple<types_t...>>;
        return static_cast<details::tuple_leaf<idx_to_reach, type_t> &>(t).content;
    }

    /**
     * Retrieves a lvalue reference to the member of a tuple at an index
     * @tparam idx_to_reach The index to reach
     * @tparam T1 type_t of the first component
     * @tparam T2 type_t of the second component
     * @param pair The pair to access
     * @return LValue reference to the member mFirst if index is 0, mSecond if index is 1.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr const tuple_element_t<idx_to_reach, tuple<types_t...>> &get(const tuple<types_t...> &t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, tuple<types_t...>>;
        return static_cast<const details::tuple_leaf<idx_to_reach, type_t> &>(t).content;
    }

    /**
     * Retrieves a rvalue reference to the member of a tuple at an index
     * @tparam idx_to_reach The index to reach
     * @tparam types_t... types_t of the tuple
     * @param tuple The tuple to access
     * @return RValue reference to the member mFirst if index is 0, mSecond if index is 1.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr tuple_element_t<idx_to_reach, tuple<types_t...>> &&get(tuple<types_t...> &&t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, tuple<types_t...>>;
        return hud::forward<type_t>(static_cast<details::tuple_leaf<idx_to_reach, type_t> &&>(t).content);
    }

    /**
     * Retrieves a rvalue reference to the member of a tuple at an index
     * @tparam idx_to_reach The index to reach
     * @tparam types_t... types_t of the tuple
     * @param tuple The tuple to access
     * @return RValue reference to the member mFirst if index is 0, mSecond if index is 1.
     */
    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] HD_FORCEINLINE constexpr const tuple_element_t<idx_to_reach, tuple<types_t...>> &&get(const tuple<types_t...> &&t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, tuple<types_t...>>;
        return hud::forward<const type_t>(static_cast<const details::tuple_leaf<idx_to_reach, type_t> &&>(t).content);
    }

    template<usize idx_to_reach, typename... types_t>
    [[nodiscard]] constexpr auto &&piecewise_get(hud::tuple<types_t...> &&t) noexcept
    {
        using type_t = tuple_element_t<idx_to_reach, hud::tuple<types_t...>>;
        return static_cast<type_t &&>(static_cast<details::tuple_leaf<idx_to_reach, type_t> &>(t).content);
    }

    /**
     * swap specialization for tuple
     * @tparam types_t types_t of the tuple
     * @param first The first tuple to swap
     * @param second The second tuple to swap
     */
    template<typename... types_t, typename... u_types_t>
    static constexpr void swap(tuple<types_t...> &first, tuple<u_types_t...> &second) noexcept
    {
        details::tuple_swap<sizeof...(types_t)>()(first, second);
    }

    /**
     * Checks if all elements of left tuple are equal to all elements at the same index of right tuple, that is, compares all elements by operator==.
     * @tparam types_t types_t of the left tuple
     * @tparam u_types_t types_t of the right tuple
     * @param first The first tuple to compare by operator==
     * @param second The second tuple to compareby operator==
     * @return true if all elements of left tuple are equal to all elements at the same index of right tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator==(const tuple<types_t...> &left, const tuple<u_types_t...> &right) noexcept
    {
        return details::tuple_equals<sizeof...(types_t)>()(left, right);
    }

    /**
     * Checks if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, that is, compares all elements by operator==.
     * @tparam types_t types_t of the left tuple
     * @tparam u_types_t types_t of the right tuple
     * @param first The first tuple to compare by operator==
     * @param second The second tuple to compareby operator==
     * @return true if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator!=(const tuple<types_t...> &left, const tuple<u_types_t...> &right) noexcept
    {
        return !(left == right);
    }

    /**
     * Checks if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, that is, compares all elements by operator==.
     * @tparam types_t types_t of the tuple
     * @param first The first tuple to compare by operator==
     * @param second The second tuple to compareby operator==
     * @return true if at least one elements of left tuple is not equals to the index corresponding elements of right tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator<(const tuple<types_t...> &left, const tuple<u_types_t...> &right) noexcept
    {
        return details::tuple_less<sizeof...(types_t)>()(left, right);
    }

    /**
     * Lexicographically compares all elements of left tuple are greater than all elements at the same index of right tuple, that is, compares all elements by operator<.
     * @tparam types_t types_t of the tuple
     * @param first The first tuple to compare by operator<
     * @param second The second tuple to compare by operator<
     * @return true if all elements of left tuple are greater than all elements at the same index of right tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator>(const tuple<types_t...> &left, const tuple<u_types_t...> &right) noexcept
    {
        return right < left;
    }

    /**
     * Lexicographically compares all elements of this tuple are less or equals all elements at the same index of another tuple, that is, compares all elements by operator<.
     * @tparam types_t types_t of the tuple
     * @param first The first tuple to compare by operator<
     * @param second The second tuple to compare by operator<
     * @return true all elements of this tuple is lexicographically less or equals the other elements at the same index, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator<=(const tuple<types_t...> &left, const tuple<u_types_t...> &right) noexcept
    {
        return !(left > right);
    }

    /**
     * Lexicographically compares all elements of left tuple are greater or equals than all elements at the same index of right tuple, that is, compares all elements by operator<.
     * @tparam types_t types_t of the tuple
     * @param first The first tuple to compare by operator<
     * @param second The second tuple to compare by operator<
     * @return true if all elements of left tuple are greater or equals than all elements at the same index of right tuple, false otherwise
     */
    template<typename... types_t, typename... u_types_t>
    [[nodiscard]] constexpr bool operator>=(const tuple<types_t...> &left, const tuple<u_types_t...> &right) noexcept
    {
        return !(left < right);
    }

    /**
     * Creates a tuple object, deducing the target type from the types of arguments.
     * @tparam types_t List of tuple types
     * @param args tuple constructor arguments list
     * @return tuple<types_t...> instance.
     */
    template<typename... types_t>
    [[nodiscard]] constexpr tuple<types_t...> make_tuple(types_t &&...args) noexcept
    {
        return tuple<types_t...>(hud::forward<types_t>(args)...);
    }

    /**
     * Constructs a tuple object with rvalue references to the elements in args suitable to be forwarded as argument to a function.
     * This function is designed to forward arguments, not to store its result in a named variable, since the returned object may contain references to temporary variables.
     */
    template<typename... types_t>
    [[nodiscard]] constexpr tuple<types_t &&...> forward_as_tuple(types_t &&...args) noexcept
    {
        return tuple<types_t &&...>(hud::forward<types_t>(args)...);
    }

    /**
     * Constructs a tuple that is a concatenation of all tuples in args in the same order.
     * @tparam tuples_t All tuple types
     * @param args Zero or more tuple to concatenate
     * @return The concatenated tuple
     */
    template<typename... tuples_t>
    constexpr typename details::tuple_cat<tuples_t...>::return_type tuple_cat(tuples_t &&...args) noexcept
    {
        using tuple_cat_result = details::tuple_cat<tuples_t...>;
        return tuple_cat_result::concatenate(forward_as_tuple(hud::forward<tuples_t>(args)...));
    }

} // namespace hud

namespace std
{
    template<typename... types_t>
    struct tuple_size<hud::tuple<types_t...>>
        : hud::tuple_size<typename hud::tuple<types_t...>>
    {
    };

    template<std::size_t index, typename... types_t>
    struct tuple_element<index, hud::tuple<types_t...>>
        : hud::tuple_element<index, hud::tuple<types_t...>>
    {
    };

} // namespace std
#endif // HD_INC_CORE_TUPLE_H