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

GTEST_TEST(math, fpclassify_f32)
{
    hud_assert_eq(hud::math::fpclassify(f32(1.0f)), hud::math::fpclassify_e::normal);
    hud_assert_eq(hud::math::fpclassify(f32(0.0f)), hud::math::fpclassify_e::zero);
    hud_assert_eq(hud::math::fpclassify(f32(-0.0f)), hud::math::fpclassify_e::zero);
    hud_assert_eq(hud::math::fpclassify(std::numeric_limits<f32>::infinity()), hud::math::fpclassify_e::infinite);
    hud_assert_eq(hud::math::fpclassify(-std::numeric_limits<f32>::infinity()), hud::math::fpclassify_e::infinite);
    hud_assert_eq(hud::math::fpclassify(std::numeric_limits<f32>::quiet_NaN()), hud::math::fpclassify_e::nan);
    hud_assert_eq(hud::math::fpclassify(f32(1e-40f)), hud::math::fpclassify_e::subnormal);
}

GTEST_TEST(math, fpclassify_f64)
{
    hud_assert_eq(hud::math::fpclassify(f64(1.0)), hud::math::fpclassify_e::normal);
    hud_assert_eq(hud::math::fpclassify(f64(0.0)), hud::math::fpclassify_e::zero);
    hud_assert_eq(hud::math::fpclassify(f64(-0.0)), hud::math::fpclassify_e::zero);
    hud_assert_eq(hud::math::fpclassify(std::numeric_limits<f64>::infinity()), hud::math::fpclassify_e::infinite);
    hud_assert_eq(hud::math::fpclassify(-std::numeric_limits<f64>::infinity()), hud::math::fpclassify_e::infinite);
    hud_assert_eq(hud::math::fpclassify(std::numeric_limits<f64>::quiet_NaN()), hud::math::fpclassify_e::nan);
    hud_assert_eq(hud::math::fpclassify(f64(1e-320)), hud::math::fpclassify_e::subnormal);
}

GTEST_TEST(math, is_finite_f32)
{
    hud_assert_true(hud::math::is_finite(f32(1.0f)));
    hud_assert_true(hud::math::is_finite(f32(0.0f)));
    hud_assert_true(hud::math::is_finite(f32(-1.0f)));
    hud_assert_true(hud::math::is_finite(f32(-0.0f)));
    hud_assert_false(hud::math::is_finite(std::numeric_limits<f32>::infinity()));
    hud_assert_false(hud::math::is_finite(std::numeric_limits<f32>::quiet_NaN()));
    hud_assert_true(hud::math::is_finite(f32(1e-40f)));
}

GTEST_TEST(math, is_finite_f64)
{
    hud_assert_true(hud::math::is_finite(f64(1.0)));
    hud_assert_true(hud::math::is_finite(f64(0.0)));
    hud_assert_true(hud::math::is_finite(f64(-1.0)));
    hud_assert_true(hud::math::is_finite(f64(-0.0)));
    hud_assert_false(hud::math::is_finite(std::numeric_limits<f64>::infinity()));
    hud_assert_false(hud::math::is_finite(std::numeric_limits<f64>::quiet_NaN()));
    hud_assert_true(hud::math::is_finite(f64(1e-320)));
}

GTEST_TEST(math, is_nan_f32)
{
    hud_assert_false(hud::math::is_nan(f32(1.0f)));
    hud_assert_false(hud::math::is_nan(f32(0.0f)));
    hud_assert_false(hud::math::is_nan(f32(-1.0f)));
    hud_assert_false(hud::math::is_nan(f32(-0.0f)));
    hud_assert_false(hud::math::is_nan(std::numeric_limits<f32>::infinity()));
    hud_assert_true(hud::math::is_nan(std::numeric_limits<f32>::quiet_NaN()));
    hud_assert_false(hud::math::is_nan(f32(1e-40f)));
}

GTEST_TEST(math, is_nan_f64)
{
    hud_assert_false(hud::math::is_nan(f64(1.0)));
    hud_assert_false(hud::math::is_nan(f64(0.0)));
    hud_assert_false(hud::math::is_nan(f64(-1.0)));
    hud_assert_false(hud::math::is_nan(f64(-0.0)));
    hud_assert_false(hud::math::is_nan(std::numeric_limits<f64>::infinity()));
    hud_assert_true(hud::math::is_nan(std::numeric_limits<f64>::quiet_NaN()));
    hud_assert_false(hud::math::is_nan(f64(1e-320)));
}

GTEST_TEST(math, is_normal_f32)
{
    hud_assert_true(hud::math::is_normal(f32(1.0f)));
    hud_assert_false(hud::math::is_normal(f32(0.0f)));
    hud_assert_true(hud::math::is_normal(f32(-1.0f)));
    hud_assert_false(hud::math::is_normal(f32(-0.0f)));
    hud_assert_false(hud::math::is_normal(std::numeric_limits<f32>::infinity()));
    hud_assert_false(hud::math::is_normal(std::numeric_limits<f32>::quiet_NaN()));
    hud_assert_false(hud::math::is_normal(f32(1e-40f)));
}

GTEST_TEST(math, is_normal_f64)
{
    hud_assert_true(hud::math::is_normal(f64(1.0)));
    hud_assert_false(hud::math::is_normal(f64(0.0)));
    hud_assert_true(hud::math::is_normal(f64(-1.0)));
    hud_assert_false(hud::math::is_normal(f64(-0.0)));
    hud_assert_false(hud::math::is_normal(std::numeric_limits<f64>::infinity()));
    hud_assert_false(hud::math::is_normal(std::numeric_limits<f64>::quiet_NaN()));
    hud_assert_false(hud::math::is_normal(f64(1e-320)));
}

GTEST_TEST(math, abs)
{
    // i32
    hud_assert_eq(hud::math::abs(i32(5)), 5);  // Positive value
    hud_assert_eq(hud::math::abs(i32(-5)), 5); // Negative value
    hud_assert_eq(hud::math::abs(i32(0)), 0);  // Zero

    // f32
    hud_assert_eq(hud::math::abs(f32(5.5f)), 5.5f);  // Positive float value
    hud_assert_eq(hud::math::abs(f32(-5.5f)), 5.5f); // Negative float value
    hud_assert_eq(hud::math::abs(f32(0.0f)), 0.0f);  // Zero as float

    // f64
    hud_assert_eq(hud::math::abs(f64(5.5f)), 5.5f);  // Positive float value
    hud_assert_eq(hud::math::abs(f64(-5.5f)), 5.5f); // Negative float value
    hud_assert_eq(hud::math::abs(f64(0.0f)), 0.0f);  // Zero as float
}