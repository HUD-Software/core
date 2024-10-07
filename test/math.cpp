#include <core/math.h>
#include <core/limits.h>

GTEST_TEST(math, limits)
{
    hud_assert_eq(hud::limits<u8>::max, std::numeric_limits<u8>::max());
    hud_assert_eq(hud::limits<u8>::min, std::numeric_limits<u8>::min());
    hud_assert_eq(hud::limits<u16>::max, std::numeric_limits<u16>::max());
    hud_assert_eq(hud::limits<u16>::min, std::numeric_limits<u16>::min());
    hud_assert_eq(hud::limits<u32>::max, std::numeric_limits<u32>::max());
    hud_assert_eq(hud::limits<u32>::min, std::numeric_limits<u32>::min());
    hud_assert_eq(hud::limits<u64>::max, std::numeric_limits<u64>::max());
    hud_assert_eq(hud::limits<u64>::min, std::numeric_limits<u64>::min());

    hud_assert_eq(hud::limits<i8>::max, std::numeric_limits<i8>::max());
    hud_assert_eq(hud::limits<i8>::min, std::numeric_limits<i8>::min());
    hud_assert_eq(hud::limits<i16>::max, std::numeric_limits<i16>::max());
    hud_assert_eq(hud::limits<i16>::min, std::numeric_limits<i16>::min());
    hud_assert_eq(hud::limits<i32>::max, std::numeric_limits<i32>::max());
    hud_assert_eq(hud::limits<i32>::min, std::numeric_limits<i32>::min());
    hud_assert_eq(hud::limits<i64>::max, std::numeric_limits<i64>::max());
    hud_assert_eq(hud::limits<i64>::min, std::numeric_limits<i64>::min());

    hud_assert_eq(hud::limits<ansichar>::max, std::numeric_limits<ansichar>::max());
    hud_assert_eq(hud::limits<ansichar>::min, std::numeric_limits<ansichar>::min());

    hud_assert_eq(hud::limits<wchar>::max, std::numeric_limits<wchar>::max());
    hud_assert_eq(hud::limits<wchar>::min, std::numeric_limits<wchar>::min());

    hud_assert_eq(hud::limits<char16>::max, std::numeric_limits<char16>::max());
    hud_assert_eq(hud::limits<char16>::min, std::numeric_limits<char16>::min());

    hud_assert_eq(hud::limits<char32>::max, std::numeric_limits<char32>::max());
    hud_assert_eq(hud::limits<char32>::min, std::numeric_limits<char32>::min());

    hud_assert_eq(hud::limits<f32>::max, std::numeric_limits<f32>::max());
    hud_assert_eq(hud::limits<f32>::min, std::numeric_limits<f32>::lowest());
    hud_assert_eq(hud::f32_min_positive, std::numeric_limits<f32>::min());
    hud_assert_eq(hud::limits<f64>::max, std::numeric_limits<f64>::max());
    hud_assert_eq(hud::limits<f64>::min, std::numeric_limits<f64>::lowest());
    hud_assert_eq(hud::f64_min_positive, std::numeric_limits<f64>::min());

    hud_assert_eq(hud::limits<uptr>::max, std::numeric_limits<uptr>::max());
    hud_assert_eq(hud::limits<uptr>::min, std::numeric_limits<uptr>::min());
    hud_assert_eq(hud::limits<iptr>::max, std::numeric_limits<iptr>::max());
    hud_assert_eq(hud::limits<iptr>::min, std::numeric_limits<iptr>::min());

    hud_assert_eq(hud::limits<usize>::max, std::numeric_limits<usize>::max());
    hud_assert_eq(hud::limits<usize>::min, std::numeric_limits<usize>::min());
    hud_assert_eq(hud::limits<isize>::max, std::numeric_limits<isize>::max());
    hud_assert_eq(hud::limits<isize>::min, std::numeric_limits<isize>::min());

#if defined(HD_INTRINSIC_INT128_SUPPORTED)
    hud_assert_eq(hud::limits<i128>::max, std::numeric_limits<__int128>::max());
    hud_assert_eq(hud::limits<i128>::min, std::numeric_limits<__int128>::min());
    hud_assert_eq(hud::limits<u128>::max, std::numeric_limits<unsigned __int128>::max());
    hud_assert_eq(hud::limits<u128>::min, std::numeric_limits<unsigned __int128>::min());

#endif
}

GTEST_TEST(math, is_finite)
{
    hud_assert_true(hud::math::is_finite((f32)0.0f));
    hud_assert_true(hud::math::is_finite((f64)0.0));
    hud_assert_true(hud::math::is_finite((f32)1.2f));
    hud_assert_true(hud::math::is_finite((f64)1.2));

    // Log(0) == infinite
    hud_assert_false(hud::math::is_finite(hud::math::log((f32)0.0f)));
    hud_assert_false(hud::math::is_finite(hud::math::log((f64)0.0)));

    // sqrt (-1) == NaN
    hud_assert_false(hud::math::is_finite(hud::math::sqrt((f32)-1.f)));
    hud_assert_false(hud::math::is_finite(hud::math::sqrt((f64)-1.0)));
}

GTEST_TEST(math, is_power_of_two)
{
    // signed integer
    hud_assert_false(hud::math::is_power_of_two(0));
    hud_assert_true(hud::math::is_power_of_two(1));
    hud_assert_true(hud::math::is_power_of_two(2));
    hud_assert_false(hud::math::is_power_of_two(3));
    hud_assert_true(hud::math::is_power_of_two(4));

    // unsigned integer
    hud_assert_false(hud::math::is_power_of_two(0u));
    hud_assert_true(hud::math::is_power_of_two(1u));
    hud_assert_true(hud::math::is_power_of_two(2u));
    hud_assert_false(hud::math::is_power_of_two(3u));
    hud_assert_true(hud::math::is_power_of_two(4u));
}

GTEST_TEST(math, is_in_range_inclusive)
{
    // signed integer
    hud_assert_false(hud::math::is_in_range_inclusive(-2, -1, 1));
    hud_assert_true(hud::math::is_in_range_inclusive(-1, -1, 1));
    hud_assert_true(hud::math::is_in_range_inclusive(0, -1, 1));
    hud_assert_true(hud::math::is_in_range_inclusive(1, -1, 1));
    hud_assert_false(hud::math::is_in_range_inclusive(2, -1, 1));

    // unsigned integer
    hud_assert_false(hud::math::is_in_range_inclusive(0u, 1u, 2u));
    hud_assert_true(hud::math::is_in_range_inclusive(1u, 1u, 2u));
    hud_assert_true(hud::math::is_in_range_inclusive(2u, 1u, 2u));
    hud_assert_false(hud::math::is_in_range_inclusive(3u, 1u, 2u));

    // 32 bits floating point
    hud_assert_false(hud::math::is_in_range_inclusive(-2.0f, -1.0f, 1.0f));
    hud_assert_false(hud::math::is_in_range_inclusive(-1.000001f, -1.0f, 1.0f));
    hud_assert_true(hud::math::is_in_range_inclusive(-1.0f, -1.0f, 1.0f));
    hud_assert_true(hud::math::is_in_range_inclusive(0.0f, -1.0f, 1.0f));
    hud_assert_true(hud::math::is_in_range_inclusive(1.0f, -1.0f, 1.0f));
    hud_assert_false(hud::math::is_in_range_inclusive(1.000001f, -1.0f, 1.0f));
    hud_assert_false(hud::math::is_in_range_inclusive(2.0f, -1.0f, 1.0f));

    // 64 bits floating point
    hud_assert_false(hud::math::is_in_range_inclusive(-2.0, -1.0, 1.0));
    hud_assert_false(hud::math::is_in_range_inclusive(-1.000001, -1.0, 1.0));
    hud_assert_true(hud::math::is_in_range_inclusive(-1.0, -1.0, 1.0));
    hud_assert_true(hud::math::is_in_range_inclusive(0.0, -1.0, 1.0));
    hud_assert_true(hud::math::is_in_range_inclusive(1.0, -1.0, 1.0));
    hud_assert_false(hud::math::is_in_range_inclusive(1.000001, -1.0, 1.0));
    hud_assert_false(hud::math::is_in_range_inclusive(2.0, -1.0, 1.0));
}

GTEST_TEST(math, floor_log2)
{
    hud_assert_eq(hud::math::floor_log2((u32)0), 0);
    hud_assert_eq(hud::math::floor_log2((u64)0), 0);

    hud_assert_eq(hud::math::floor_log2((u32)1), 0);
    hud_assert_eq(hud::math::floor_log2((u64)1), 0);
    hud_assert_eq(hud::math::floor_log2((u32)2), 1);
    hud_assert_eq(hud::math::floor_log2((u64)2), 1);
    hud_assert_eq(hud::math::floor_log2((u32)4), 2);
    hud_assert_eq(hud::math::floor_log2((u64)4), 2);
    hud_assert_eq(hud::math::floor_log2((u32)8), 3);
    hud_assert_eq(hud::math::floor_log2((u64)8), 3);
    hud_assert_eq(hud::math::floor_log2((u32)16), 4);
    hud_assert_eq(hud::math::floor_log2((u64)16), 4);
    hud_assert_eq(hud::math::floor_log2((u32)32), 5);
    hud_assert_eq(hud::math::floor_log2((u64)32), 5);
    hud_assert_eq(hud::math::floor_log2((u32)64), 6);
    hud_assert_eq(hud::math::floor_log2((u64)64), 6);
    hud_assert_eq(hud::math::floor_log2((u32)128), 7);
    hud_assert_eq(hud::math::floor_log2((u64)128), 7);
    hud_assert_eq(hud::math::floor_log2((u32)256), 8);
    hud_assert_eq(hud::math::floor_log2((u64)256), 8);
    hud_assert_eq(hud::math::floor_log2((u32)512), 9);
    hud_assert_eq(hud::math::floor_log2((u64)512), 9);
    hud_assert_eq(hud::math::floor_log2((u32)1024), 10);
    hud_assert_eq(hud::math::floor_log2((u64)1024), 10);

    hud_assert_eq(hud::math::floor_log2((u32)3), 1);    // log2(3) ≈ 1.585
    hud_assert_eq(hud::math::floor_log2((u64)3), 1);    // log2(3) ≈ 1.585
    hud_assert_eq(hud::math::floor_log2((u32)5), 2);    // log2(5) ≈ 2.322
    hud_assert_eq(hud::math::floor_log2((u64)5), 2);    // log2(5) ≈ 2.322
    hud_assert_eq(hud::math::floor_log2((u32)6), 2);    // log2(6) ≈ 2.585
    hud_assert_eq(hud::math::floor_log2((u64)6), 2);    // log2(6) ≈ 2.585
    hud_assert_eq(hud::math::floor_log2((u32)9), 3);    // log2(9) ≈ 3.169
    hud_assert_eq(hud::math::floor_log2((u64)9), 3);    // log2(9) ≈ 3.169
    hud_assert_eq(hud::math::floor_log2((u32)100), 6);  // log2(100) ≈ 6.644
    hud_assert_eq(hud::math::floor_log2((u64)100), 6);  // log2(100) ≈ 6.644
    hud_assert_eq(hud::math::floor_log2((u32)1000), 9); // log2(1000) ≈ 9.965
    hud_assert_eq(hud::math::floor_log2((u64)1000), 9); // log2(1000) ≈ 9.965

    hud_assert_eq(hud::math::floor_log2((u32)1023), 9);        // log2(1023) ≈ 9.993
    hud_assert_eq(hud::math::floor_log2((u64)1023), 9);        // log2(1023) ≈ 9.993
    hud_assert_eq(hud::math::floor_log2((u32)65535), 15);      // log2(65535) ≈ 15.999
    hud_assert_eq(hud::math::floor_log2((u64)65535), 15);      // log2(65535) ≈ 15.999
    hud_assert_eq(hud::math::floor_log2((u32)1048576), 20);    // log2(1048576) = 20
    hud_assert_eq(hud::math::floor_log2((u64)1048576), 20);    // log2(1048576) = 20
    hud_assert_eq(hud::math::floor_log2((u32)2147483647), 30); // log2(2147483647) ≈ 30.999
    hud_assert_eq(hud::math::floor_log2((u64)2147483647), 30); // log2(2147483647) ≈ 30.999
}

GTEST_TEST(math, count_leading_zero_u8)
{
    // Special case : 0
    hud_assert_eq(hud::math::count_leading_zero(u8(0)), 8);

    // Limit case : 1 -> 7 zeros
    hud_assert_eq(hud::math::count_leading_zero(u8(1)), 7);

    // Limit case : 2 -> 6 zeros
    hud_assert_eq(hud::math::count_leading_zero(u8(2)), 6);

    // Case for 255
    hud_assert_eq(hud::math::count_leading_zero(u8(255)), 0);

    // Middle case : 128 -> 0b10000000 (1 zero)
    hud_assert_eq(hud::math::count_leading_zero(u8(128)), 0);

    // Middle case : 64 -> 0b01000000 (1 zero)
    hud_assert_eq(hud::math::count_leading_zero(u8(64)), 1);

    // Other cases
    hud_assert_eq(hud::math::count_leading_zero(u8(15)), 4); // 0b00001111 -> 4 zeros
    hud_assert_eq(hud::math::count_leading_zero(u8(16)), 3); // 0b00010000 -> 3 zeros
}

GTEST_TEST(math, count_leading_zero_u32)
{
    // Special case : 0
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(0)), 32);

    // Limit case : 1 -> 31 zéros
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(1)), 31);

    // Limit case : 2 -> 30 zéros
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(2)), 30);

    // Case for 0xFFFFFFFF
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(0xFFFFFFFF)), 0);

    // Middle case : 0x80000000 -> 1 zéro
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(0x80000000)), 0);

    // Middle case : 0x40000000 -> 1 zéro
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(0x40000000)), 1);

    // Other cases
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(0x0F000000)), 4); // 4 zeros
    hud_assert_eq(hud::math::count_leading_zero(uint32_t(0x10000000)), 3); // 3 zeros
}

GTEST_TEST(math, count_leading_zero_u64)
{
    // Special case: 0
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0)), 64);

    // Edge case: 1 -> 63 leading zeros
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(1)), 63);

    // Edge case: 2 -> 62 leading zeros
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(2)), 62);

    // Case for 0xFFFFFFFFFFFFFFFF (no leading zeros)
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0xFFFFFFFFFFFFFFFF)), 0);

    // Intermediate case: 0x8000000000000000 -> 1 leading zero
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x8000000000000000)), 0);

    // Intermediate case: 0x4000000000000000 -> 1 leading zero
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x4000000000000000)), 1);

    // Case for 0x00000000FFFFFFFF -> 32 leading zeros
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x00000000FFFFFFFF)), 32);

    // Case for 0x0000FFFF00000000 -> 16 leading zeros
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x0000FFFF00000000)), 16);

    // Case for 0x00F0000000000000 -> 8 leading zeros
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x00F0000000000000)), 8);

    // Other intermediate cases
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x0F00000000000000)), 4); // 4 leading zeros
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x1000000000000000)), 3); // 3 leading zeros
    hud_assert_eq(hud::math::count_leading_zero(uint64_t(0x2000000000000000)), 2); // 2 leading zeros
}