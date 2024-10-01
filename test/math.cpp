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

GTEST_TEST(math, log)
{
}

GTEST_TEST(math, sqrt)
{
}