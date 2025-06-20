#ifndef HD_INC_CORE_TEMPLATES_INTEGER_SEQUENCE_H
#define HD_INC_CORE_TEMPLATES_INTEGER_SEQUENCE_H
#include "../traits/is_integral.h"
#include "../traits/common_type.h"

namespace hud
{

    /** Represents a compile-time sequence of integers. When used as an argument to a function template, the parameter pack Ints can be deduced and used in pack expansion. */
    template<typename type_t, type_t... ints>
    struct integer_sequence
    {
        using value_type = type_t;

        static_assert(is_integral_v<value_type>, "hdIntegralSequence can only be instantiated with an integral type");

        static constexpr usize size() noexcept
        {
            return sizeof...(ints);
        }
    };

    /** Helper alias template defined for the common case where type_t is usize. */
    template<usize... ints>
    using index_sequence = integer_sequence<usize, ints...>;

    namespace details
    {
        // Stores a tuple of indices. Used by tuple and pair, and by bind() to
        // extract the elements in a tuple.
        template<usize... indices>
        struct index_tuple
        {
            using next_type = index_tuple<indices..., sizeof...(indices)>;
        };

        // Builds an index_tuple<0, 1, 2, ..., num-1>.
        template<usize num>
        struct build_index_tuple
        {
            using value_type = typename build_index_tuple<num - 1u>::value_type::next_type;
        };

        template<>
        struct build_index_tuple<0u>
        {
            using value_type = index_tuple<>;
        };

        template<typename type_t, type_t num, typename Sequence = typename build_index_tuple<num>::value_type>
        struct make_integer_sequence;

        template<typename type_t, type_t num, usize... int_index>
        struct make_integer_sequence<type_t, num, index_tuple<int_index...>>
        {
            static_assert(num >= 0, "Cannot make integer sequence of negative length");
            using value_type = integer_sequence<type_t, static_cast<type_t>(int_index)...>;
        };
    } // namespace details

    /** Helper alias template defined to simplify creation of integer_sequence types with 0, 1, 2, ..., N-1 as Ints. */
    template<typename type_t, type_t num>
    using make_integer_sequence = typename details::make_integer_sequence<type_t, num>::value_type;

    /** Helper alias template defined to simplify creation of index_sequence types with 0, 1, 2, ..., N-1 as Ints. */
    template<usize num>
    using make_index_sequence = make_integer_sequence<usize, num>;

    /** Helper alias template index_sequence is defined to convert any type parameter pack into an index sequence of the same length. */
    template<typename... args_t>
    using make_index_sequence_for = make_index_sequence<sizeof...(args_t)>;

    /** Contenate IntegerSequences into one integer_sequence */
    template<typename... IntSequences>
    struct cat_index_sequence;

    template<typename type1_t, type1_t... seq_1, typename type2_t, type2_t... seq_2, typename... rest_t>
    struct cat_index_sequence<integer_sequence<type1_t, seq_1...>, integer_sequence<type2_t, seq_2...>, rest_t...>
    { // Concatenate 2 integer_sequence with the followings
        using value_type = typename cat_index_sequence<typename cat_index_sequence<integer_sequence<type1_t, seq_1...>, integer_sequence<type2_t, seq_2...>>::value_type, rest_t...>::value_type;
    };

    template<typename type_t, type_t... ints>
    struct cat_index_sequence<integer_sequence<type_t, ints...>>
    { // Concatenate 1 integer_sequence
        using value_type = integer_sequence<type_t, ints...>;
    };

    template<typename type_t, type_t... seq_1, type_t... seq_2>
    struct cat_index_sequence<integer_sequence<type_t, seq_1...>, integer_sequence<type_t, seq_2...>>
    { // Concantenante 2 integer_sequence with same type
        using value_type = integer_sequence<type_t, seq_1..., seq_2...>;
    };

    template<typename type1_t, type1_t... seq_1, typename type2_t, type2_t... seq_2>
    struct cat_index_sequence<integer_sequence<type1_t, seq_1...>, integer_sequence<type2_t, seq_2...>>
    { // Concantenante 2 integer_sequence with different types
        using commont_type = common_type_t<type1_t, type2_t>;
        using value_type = integer_sequence<commont_type, seq_1..., seq_2...>;
    };

    /** Helper alias template equivalent to cat_index_sequence<Sequences...>::value_type. */
    template<typename... Sequences>
    using cat_integer_sequence_t = typename cat_index_sequence<Sequences...>::value_type;

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_INTEGER_SEQUENCE_H
