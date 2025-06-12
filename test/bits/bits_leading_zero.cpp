#include <core/bits.h>

GTEST_TEST(bits, leading_zero_u8)
{
    hud_assert_eq(hud::bits::leading_zeros(u8(0)), 8);
    hud_assert_eq(hud::bits::leading_zeros(u8(1)), 7);
    hud_assert_eq(hud::bits::leading_zeros(u8(2)), 6);
    hud_assert_eq(hud::bits::leading_zeros(u8(0xFF)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u8(0x80)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u8(0x40)), 1);
    hud_assert_eq(hud::bits::leading_zeros(u8(0x0F)), 4);
    hud_assert_eq(hud::bits::leading_zeros(u8(0x10)), 3);
}

GTEST_TEST(bits, leading_zero_u16)
{
    hud_assert_eq(hud::bits::leading_zeros(u16(0)), 16);
    hud_assert_eq(hud::bits::leading_zeros(u16(1)), 15);
    hud_assert_eq(hud::bits::leading_zeros(u16(2)), 14);
    hud_assert_eq(hud::bits::leading_zeros(u16(0xFFFF)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u16(0x8000)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u16(0x4000)), 1);
    hud_assert_eq(hud::bits::leading_zeros(u16(0x0F00)), 4);
    hud_assert_eq(hud::bits::leading_zeros(u16(0x1000)), 3);
}

GTEST_TEST(bits, leading_zero_u32)
{
    hud_assert_eq(hud::bits::leading_zeros(u32(0)), 32);
    hud_assert_eq(hud::bits::leading_zeros(u32(1)), 31);
    hud_assert_eq(hud::bits::leading_zeros(u32(2)), 30);
    hud_assert_eq(hud::bits::leading_zeros(u32(0xFFFFFFFF)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u32(0x80000000)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u32(0x40000000)), 1);
    hud_assert_eq(hud::bits::leading_zeros(u32(0x0F000000)), 4);
    hud_assert_eq(hud::bits::leading_zeros(u32(0x10000000)), 3);
}

GTEST_TEST(bits, leading_zero_u64)
{
    hud_assert_eq(hud::bits::leading_zeros(u64(0)), 64);
    hud_assert_eq(hud::bits::leading_zeros(u64(1)), 63);
    hud_assert_eq(hud::bits::leading_zeros(u64(2)), 62);
    hud_assert_eq(hud::bits::leading_zeros(u64(0xFFFFFFFFFFFFFFFF)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x8000000000000000)), 0);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x4000000000000000)), 1);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x00000000FFFFFFFF)), 32);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x0000FFFF00000000)), 16);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x00F0000000000000)), 8);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x0F00000000000000)), 4);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x1000000000000000)), 3);
    hud_assert_eq(hud::bits::leading_zeros(u64(0x2000000000000000)), 2);
}