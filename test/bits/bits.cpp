#include <core/bits.h>

GTEST_TEST(bits, reverse_bits)
{

    // Non constant
    {
        const u32 reverse_bytes = hud::bits::reverse_bits(0x04C11DB7u);
        hud_assert_eq(reverse_bytes, 0xEDB88320);
    }

    // Constant
    {
        constexpr u32 reverse_bytes = hud::bits::reverse_bits(0x04C11DB7u);
        hud_assert_eq(reverse_bytes, 0xEDB88320);
    }
}

GTEST_TEST(bits, has_zero_byte)
{

    // Non constant
    {
        hud_assert_eq(hud::bits::has_zero_byte(0x1213140015001617u), 0x0000008000800000u);
        hud_assert_eq(hud::bits::has_zero_byte(0xFFFFFFFFFFFFFFFFu), 0x0000000000000000u);
        hud_assert_eq(hud::bits::has_zero_byte(0x0000000000000000u), 0x8080808080808080u);
        hud_assert_eq(hud::bits::has_zero_byte(0x1010101010101000u), 0x0000000000000080u);
        hud_assert_eq(hud::bits::has_zero_byte(0x1010101010100010u), 0x0000000000008000u);
        hud_assert_eq(hud::bits::has_zero_byte(0x1010101010001010u), 0x0000000000800000u);
        hud_assert_eq(hud::bits::has_zero_byte(0x1010101000101010u), 0x0000000080000000u);
        hud_assert_eq(hud::bits::has_zero_byte(0x1010100010101010u), 0x0000008000000000u);
        hud_assert_eq(hud::bits::has_zero_byte(0x1010001010101010u), 0x0000800000000000u);
        hud_assert_eq(hud::bits::has_zero_byte(0x1000101010101010u), 0x0080000000000000u);
        hud_assert_eq(hud::bits::has_zero_byte(0x0010101010101010u), 0x8000000000000000u);
    }

    // Constant
    {
        constexpr u64 result = hud::bits::has_zero_byte(0x1213140015001617u);
        hud_assert_eq(result, 0x0000008000800000u);
        constexpr u64 result_0 = hud::bits::has_zero_byte(0xFFFFFFFFFFFFFFFFu);
        hud_assert_eq(result_0, 0x0000000000000000u);
        constexpr u64 result_1 = hud::bits::has_zero_byte(0x0000000000000000u);
        hud_assert_eq(result_1, 0x8080808080808080u);
        constexpr u64 result_2 = hud::bits::has_zero_byte(0x1010101010101000u);
        hud_assert_eq(result_2, 0x0000000000000080u);
        constexpr u64 result_3 = hud::bits::has_zero_byte(0x1010101010100010u);
        hud_assert_eq(result_3, 0x0000000000008000u);
        constexpr u64 result_4 = hud::bits::has_zero_byte(0x1010101010001010u);
        hud_assert_eq(result_4, 0x0000000000800000u);
        constexpr u64 result_5 = hud::bits::has_zero_byte(0x1010101000101010u);
        hud_assert_eq(result_5, 0x0000000080000000u);
        constexpr u64 result_6 = hud::bits::has_zero_byte(0x1010100010101010u);
        hud_assert_eq(result_6, 0x0000008000000000u);
        constexpr u64 result_7 = hud::bits::has_zero_byte(0x1010001010101010u);
        hud_assert_eq(result_7, 0x0000800000000000u);
        constexpr u64 result_8 = hud::bits::has_zero_byte(0x1000101010101010u);
        hud_assert_eq(result_8, 0x0080000000000000u);
        constexpr u64 result_9 = hud::bits::has_zero_byte(0x0010101010101010u);
        hud_assert_eq(result_9, 0x8000000000000000u);
    }
}

GTEST_TEST(bits, has_value_byte)
{

    // Non constant
    {
        hud_assert_eq(hud::bits::has_value_byte(0x1213140015001217u, 0x12), 0x8000000000008000u);
        hud_assert_eq(hud::bits::has_value_byte(0xFFFFFFFFFFFFFFFFu, 0x12), 0x0000000000000000u);
        hud_assert_eq(hud::bits::has_value_byte(0x0000000000000000u, 0x12), 0x0000000000000000u);
        hud_assert_eq(hud::bits::has_value_byte(0x1010101010101012u, 0x12), 0x0000000000000080u);
        hud_assert_eq(hud::bits::has_value_byte(0x1010101010101210u, 0x12), 0x0000000000008000u);
        hud_assert_eq(hud::bits::has_value_byte(0x1010101010121010u, 0x12), 0x0000000000800000u);
        hud_assert_eq(hud::bits::has_value_byte(0x1010101012101010u, 0x12), 0x0000000080000000u);
        hud_assert_eq(hud::bits::has_value_byte(0x1010101210101010u, 0x12), 0x0000008000000000u);
        hud_assert_eq(hud::bits::has_value_byte(0x1010121010101010u, 0x12), 0x0000800000000000u);
        hud_assert_eq(hud::bits::has_value_byte(0x1012101010101010u, 0x12), 0x0080000000000000u);
        hud_assert_eq(hud::bits::has_value_byte(0x1210101010101010u, 0x12), 0x8000000000000000u);
    }

    // Constant
    {
        constexpr u64 result = hud::bits::has_value_byte(0x1213140015001217u, 0x12);
        hud_assert_eq(result, 0x8000000000008000u);
        constexpr u64 result_0 = hud::bits::has_value_byte(0xFFFFFFFFFFFFFFFFu, 0x12);
        hud_assert_eq(result_0, 0x0000000000000000u);
        constexpr u64 result_1 = hud::bits::has_value_byte(0x0000000000000000u, 0x12);
        hud_assert_eq(result_1, 0x0000000000000000u);
        constexpr u64 result_2 = hud::bits::has_value_byte(0x1010101010101012u, 0x12);
        hud_assert_eq(result_2, 0x0000000000000080u);
        constexpr u64 result_3 = hud::bits::has_value_byte(0x1010101010101210u, 0x12);
        hud_assert_eq(result_3, 0x0000000000008000u);
        constexpr u64 result_4 = hud::bits::has_value_byte(0x1010101010121010u, 0x12);
        hud_assert_eq(result_4, 0x0000000000800000u);
        constexpr u64 result_5 = hud::bits::has_value_byte(0x1010101012101010u, 0x12);
        hud_assert_eq(result_5, 0x0000000080000000u);
        constexpr u64 result_6 = hud::bits::has_value_byte(0x1010101210101010u, 0x12);
        hud_assert_eq(result_6, 0x0000008000000000u);
        constexpr u64 result_7 = hud::bits::has_value_byte(0x1010121010101010u, 0x12);
        hud_assert_eq(result_7, 0x0000800000000000u);
        constexpr u64 result_8 = hud::bits::has_value_byte(0x1012101010101010u, 0x12);
        hud_assert_eq(result_8, 0x0080000000000000u);
        constexpr u64 result_9 = hud::bits::has_value_byte(0x1210101010101010u, 0x12);
        hud_assert_eq(result_9, 0x8000000000000000u);
    }
}