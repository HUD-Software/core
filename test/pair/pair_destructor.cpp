#include <core/containers/pair.h>

GTEST_TEST(pair, destructor_call_inner_components_destructors)
{
    using first_type = hud_test::SetBoolToTrueWhenDestroyed;
    using second_type = hud_test::SetBoolToTrueWhenDestroyed;

    const auto test = []()
    {
        i32 destructor_counts[2] = {0};
        bool destructor_first_ok = true;
        bool destructor_second_ok = true;
        {
            hud::pair<first_type, second_type> pair {&destructor_counts[0], &destructor_counts[1]};
            destructor_first_ok &= (destructor_counts[0] == 0);
            destructor_second_ok &= (destructor_counts[1] == 0);
        }
        destructor_first_ok &= (destructor_counts[0] == 1);
        destructor_second_ok &= (destructor_counts[1] == 1);
        return std::tuple {destructor_first_ok, destructor_second_ok};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(pair, pair_is_trivially_destructible_if_inner_types_are_trivially_destructible)
{
    static_assert(hud::is_trivially_destructible_v<hud::pair<i32, i64>>);
    hud_assert_true((hud::is_trivially_destructible_v<hud::pair<i32, i64>>));

    static_assert(!hud::is_trivially_destructible_v<hud::pair<hud_test::non_bitwise_type, i64>>);
    hud_assert_true((!hud::is_trivially_destructible_v<hud::pair<hud_test::non_bitwise_type, i64>>));

    static_assert(!hud::is_trivially_destructible_v<hud::pair<i32, hud_test::non_bitwise_type>>);
    hud_assert_true((!hud::is_trivially_destructible_v<hud::pair<i32, hud_test::non_bitwise_type>>));

    static_assert(!hud::is_trivially_destructible_v<hud::pair<hud_test::non_bitwise_type, hud_test::non_bitwise_type>>);
    hud_assert_true((!hud::is_trivially_destructible_v<hud::pair<hud_test::non_bitwise_type, hud_test::non_bitwise_type>>));
}