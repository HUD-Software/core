#include <core/i128.h>

GTEST_TEST(i128, size)
{
    hud_assert_eq(sizeof(i128), 16u);
}

GTEST_TEST(i128, signed_unsigned)
{
    hud_assert_true(hud::is_signed_v<i128>);
}

GTEST_TEST(i128, limits)
{
    hud_assert_eq(hud::i128_min.low(), std::numeric_limits<u64>::min());
    hud_assert_eq(hud::i128_min.high(), std::numeric_limits<i64>::min());
    hud_assert_eq(hud::i128_max.low(), std::numeric_limits<u64>::max());
    hud_assert_eq(hud::i128_max.high(), std::numeric_limits<i64>::max());
}