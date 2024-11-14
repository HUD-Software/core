#include <core/bits.h>

GTEST_TEST(bits, trailing_zero_u32)
{
    hud_assert_eq(hud::bits::trailing_zero(u32(0)), 32);
    hud_assert_eq(hud::bits::trailing_zero(u32(1)), 0);
    hud_assert_eq(hud::bits::trailing_zero(u32(2)), 1);
    hud_assert_eq(hud::bits::trailing_zero(u32(0xFFFFFFFF)), 0);
    hud_assert_eq(hud::bits::trailing_zero(u32(0x80000000)), 31);
    hud_assert_eq(hud::bits::trailing_zero(u32(0x40000000)), 30);
    hud_assert_eq(hud::bits::trailing_zero(u32(0x0F000000)), 24);
    hud_assert_eq(hud::bits::trailing_zero(u32(0x10000000)), 28);
}

GTEST_TEST(bits, trailing_zero_u64)
{
    hud_assert_eq(hud::bits::trailing_zero(u64(0)), 64);
    hud_assert_eq(hud::bits::trailing_zero(u64(1)), 0);
    hud_assert_eq(hud::bits::trailing_zero(u64(2)), 1);
    hud_assert_eq(hud::bits::trailing_zero(u64(0xFFFFFFFFFFFFFFFF)), 0);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x8000000000000000)), 63);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x4000000000000000)), 62);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x00000000FFFFFFFF)), 0);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x0000FFFF00000000)), 32);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x00F0000000000000)), 52);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x0F00000000000000)), 56);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x1000000000000000)), 60);
    hud_assert_eq(hud::bits::trailing_zero(u64(0x2000000000000000)), 61);
}