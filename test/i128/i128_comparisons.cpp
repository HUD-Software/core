
GTEST_TEST(i128, operator_equal)
{
    hud_assert_true(hud::i128 {0} == hud::i128 {0});
    hud_assert_false(hud::i128 {0} == hud::i128 {1});
}

GTEST_TEST(i128, operator_not_equal)
{
    hud_assert_false(hud::i128 {0} != hud::i128 {0});
    hud_assert_true(hud::i128 {0} != hud::i128 {1});
}

GTEST_TEST(i128, operator_less)
{
    hud_assert_false(hud::i128 {0} < hud::i128 {0});
    hud_assert_true(hud::i128 {0} < hud::i128 {1});
    hud_assert_false(hud::i128 {1} < hud::i128 {0});
}

GTEST_TEST(i128, operator_less_equal)
{
    hud_assert_true(hud::i128 {0} <= hud::i128 {0});
    hud_assert_true(hud::i128 {0} <= hud::i128 {1});
    hud_assert_false(hud::i128 {1} <= hud::i128 {0});
}

GTEST_TEST(i128, operator_greater)
{
    hud_assert_false(hud::i128 {0} > hud::i128 {0});
    hud_assert_false(hud::i128 {0} > hud::i128 {1});
    hud_assert_true(hud::i128 {1} > hud::i128 {0});
}

GTEST_TEST(i128, operator_greater_equal)
{
    hud_assert_true(hud::i128 {0} >= hud::i128 {0});
    hud_assert_false(hud::i128 {0} >= hud::i128 {1});
    hud_assert_true(hud::i128 {1} >= hud::i128 {0});
}