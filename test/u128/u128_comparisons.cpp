
GTEST_TEST(u128, operator_equal)
{
    hud_assert_true(hud::u128 {0} == hud::u128 {0});
    hud_assert_false(hud::u128 {0} == hud::u128 {1});
}

GTEST_TEST(u128, operator_not_equal)
{
    hud_assert_false(hud::u128 {0} != hud::u128 {0});
    hud_assert_true(hud::u128 {0} != hud::u128 {1});
}

GTEST_TEST(u128, operator_less)
{
    hud_assert_false(hud::u128 {0} < hud::u128 {0});
    hud_assert_true(hud::u128 {0} < hud::u128 {1});
    hud_assert_false(hud::u128 {1} < hud::u128 {0});
}

GTEST_TEST(u128, operator_less_equal)
{
    hud_assert_true(hud::u128 {0} <= hud::u128 {0});
    hud_assert_true(hud::u128 {0} <= hud::u128 {1});
    hud_assert_false(hud::u128 {1} <= hud::u128 {0});
}

GTEST_TEST(u128, operator_greater)
{
    hud_assert_false(hud::u128 {0} > hud::u128 {0});
    hud_assert_false(hud::u128 {0} > hud::u128 {1});
    hud_assert_true(hud::u128 {1} > hud::u128 {0});
}

GTEST_TEST(u128, operator_greater_equal)
{
    hud_assert_true(hud::u128 {0} >= hud::u128 {0});
    hud_assert_false(hud::u128 {0} >= hud::u128 {1});
    hud_assert_true(hud::u128 {1} >= hud::u128 {0});
}