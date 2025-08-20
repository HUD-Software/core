#include <core/containers/compressed_tuple.h>

GTEST_TEST(compressed_tuple, swap_non_trivially_copy_assignable_same_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_move_assignable_type, f32, i32, wchar>;

    const auto test = []()
    {
        tuple_type tuple_a {1, 12.0f, 123, L'c'};
        tuple_type tuple_b {2, 36.0f, 568, L'p'};
        hud::swap(tuple_a, tuple_b);
        return std::compressed_tuple {
            hud::get<0>(tuple_a).id() == 2,
            hud::get<0>(tuple_a).move_assign_count() == 1u,
            hud::get<0>(tuple_a).copy_assign_count() == 0u,
            hud::get<0>(tuple_a).move_constructor_count() == 0u,
            hud::get<0>(tuple_a).copy_constructor_count() == 0u,
            hud::get<1>(tuple_a) == 36.0f,
            hud::get<2>(tuple_a) == 568,
            hud::get<3>(tuple_a) == L'p',
            hud::get<0>(tuple_b).id() == 1,
            hud::get<0>(tuple_b).move_assign_count() == 1u,
            hud::get<0>(tuple_b).copy_assign_count() == 0u,
            hud::get<0>(tuple_b).move_constructor_count() == 1u,
            hud::get<0>(tuple_b).copy_constructor_count() == 0u,
            hud::get<1>(tuple_b) == 12.0f,
            hud::get<2>(tuple_b) == 123,
            hud::get<3>(tuple_b) == L'c',
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
    }
}

GTEST_TEST(compressed_tuple, swap_non_trivially_copy_assignable_different_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_move_assignable_type2, f32, i32, wchar>;
    using other_tuple_type = hud::compressed_tuple<hud_test::non_bitwise_move_assignable_type, f32, i32, wchar>;
    const auto test = []()
    {
        tuple_type tuple_a {1, 12.0f, 123, L'c'};
        other_tuple_type tuple_b {2, 36.0f, 568, L'p'};
        hud::swap(tuple_a, tuple_b);
        return std::compressed_tuple {
            hud::get<0>(tuple_a).id() == 2,
            hud::get<0>(tuple_a).move_assign_count() == 1u,
            hud::get<0>(tuple_a).copy_assign_count() == 0u,
            hud::get<0>(tuple_a).move_constructor_count() == 0u,
            hud::get<0>(tuple_a).copy_constructor_count() == 0u,
            hud::get<1>(tuple_a) == 36.0f,
            hud::get<2>(tuple_a) == 568,
            hud::get<3>(tuple_a) == L'p',
            hud::get<0>(tuple_b).id() == 1,
            hud::get<0>(tuple_b).move_assign_count() == 1u,
            hud::get<0>(tuple_b).copy_assign_count() == 0u,
            hud::get<0>(tuple_b).move_constructor_count() == 1u,
            hud::get<0>(tuple_b).copy_constructor_count() == 0u,
            hud::get<1>(tuple_b) == 12.0f,
            hud::get<2>(tuple_b) == 123,
            hud::get<3>(tuple_b) == L'c',
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
    }
}
