#include <core/templates/bit_cast.h>

GTEST_TEST(templates, bit_cast)
{

    constexpr f64 f64v = 19880124.0;
    constexpr auto u64v = hud::bit_cast<u64>(f64v);
    GTEST_ASSERT_EQ(hud::bit_cast<f64>(u64v), f64v);

    constexpr u64 u64v2 = 0x3fe9000000000000ull;
    constexpr auto f64v2 = hud::bit_cast<f64>(u64v2);
    GTEST_ASSERT_EQ(hud::bit_cast<u64>(f64v2), u64v2);
}