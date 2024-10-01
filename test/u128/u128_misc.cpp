#include <core/i128.h>

GTEST_TEST(u128, size)
{
    hud_assert_eq(sizeof(u128), 16u);
}

GTEST_TEST(u128, signed_unsigned)
{
    hud_assert_true(hud::is_unsigned_v<u128>);
}

GTEST_TEST(u128, limits)
{
    hud_assert_eq(hud::u128_min.low(), std::numeric_limits<u64>::min());
    hud_assert_eq(hud::u128_min.high(), std::numeric_limits<u64>::min());
    hud_assert_eq(hud::u128_max.low(), std::numeric_limits<u64>::max());
    hud_assert_eq(hud::u128_max.high(), std::numeric_limits<u64>::max());
}