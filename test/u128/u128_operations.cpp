GTEST_TEST(u128, addition)
{
    hud_assert_eq(hud::u128 {1} + hud::u128 {2}, hud::u128 {3});
    // Check for overflow of u128; works if it's a two's complement representation.
    hud_assert_eq(hud::u128 {1} + hud::u128_max, hud::u128 {0});
}

GTEST_TEST(u128, subtraction)
{
    hud_assert_eq(hud::u128 {3} - hud::u128 {2}, hud::u128 {1});

    // Check for underflow of u128; works if it's a two's complement representation.
    hud_assert_eq(hud::u128_min - hud::u128 {1}, hud::u128_max);
}

GTEST_TEST(u128, division)
{
    hud_assert_eq(hud::u128 {2} / hud::u128 {2}, hud::u128 {1});
    hud_assert_eq(hud::u128 {3} / hud::u128 {2}, hud::u128 {1});
}

GTEST_TEST(u128, multiplication)
{
    hud_assert_eq(hud::u128 {2} * hud::u128 {2}, hud::u128 {4});
    hud_assert_eq(hud::u128 {3} * hud::u128 {0}, hud::u128 {0});
    // Check for overflow of u128; works if it's a two's complement representation.
    hud_assert_eq(hud::u128_max * hud::u128 {2}, hud::u128_max - 1);
}

GTEST_TEST(u128, modulus)
{
    hud_assert_eq(hud::u128 {2} % hud::u128 {2}, hud::u128 {0});
    hud_assert_eq(hud::u128 {3} % hud::u128 {2}, hud::u128 {1});
    hud_assert_eq(hud::u128 {10} % hud::u128 {4}, hud::u128 {2});

    hud_assert_eq(hud::u128 {0} % hud::u128_max, hud::u128 {0});
    hud_assert_eq(hud::u128_max % hud::u128_max, hud::u128 {0});
}