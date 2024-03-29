#include <core/templates/integer_sequence.h>

GTEST_TEST(templates, integer_sequence)
{

    hud::integer_sequence<i32, 1, 2, 3, 16, 32> seq;
    hud_assert_eq(seq.size(), 5u);

    using seq1_type = hud::cat_integer_sequence_t<hud::integer_sequence<i8, 1, 2>>;
    seq1_type seq1;
    constexpr bool seq1_same_type = hud::is_same_v<seq1_type::value_type, i8>;
    hud_assert_true(seq1_same_type);
    hud_assert_eq(seq1.size(), 2u);

    using seq2_type = hud::cat_integer_sequence_t<hud::integer_sequence<i8, 1, 2>, hud::integer_sequence<i8, 3, 4, 5>>;
    seq2_type seq2;
    constexpr bool seq2_same_type = hud::is_same_v<seq2_type::value_type, i8>;
    hud_assert_true(seq2_same_type);
    hud_assert_eq(seq2.size(), 5u);

    using seq3_type = hud::cat_integer_sequence_t<hud::integer_sequence<i8, 1, 2>, hud::integer_sequence<i8, 3, 4, 5>, hud::integer_sequence<i8, 6, 7, 8>>;
    seq3_type seq3;
    constexpr bool seq3_same_type = hud::is_same_v<seq3_type::value_type, i8>;
    hud_assert_true(seq3_same_type);
    hud_assert_eq(seq3.size(), 8u);

    using seq4_type = hud::cat_integer_sequence_t<hud::integer_sequence<i8, 1, 2>, hud::integer_sequence<i32, 3, 4, 5>, hud::integer_sequence<usize, 6, 7, 8, 7>>;
    seq4_type seq4;
    constexpr bool seq4_same_type = hud::is_same_v<seq4_type::value_type, usize>;
    hud_assert_true(seq4_same_type);
    hud_assert_eq(seq4.size(), 9u);
}