#include <core/types.h>
#include <core/traits/is_signed.h>
#include <core/traits/is_unsigned.h>

GTEST_TEST(types, size)
{
    hud_assert_eq(sizeof(bool), 1u);
    hud_assert_eq(sizeof(i8), 1u);
    hud_assert_eq(sizeof(i16), 2u);
    hud_assert_eq(sizeof(i32), 4u);
    hud_assert_eq(sizeof(i64), 8u);
    hud_assert_eq(sizeof(u8), 1u);
    hud_assert_eq(sizeof(u16), 2u);
    hud_assert_eq(sizeof(u32), 4u);
    hud_assert_eq(sizeof(u64), 8u);
    hud_assert_eq(sizeof(ansichar), 1u);

    if constexpr (hud::compilation::is_os(hud::os_e::windows))
    {
        hud_assert_eq(sizeof(wchar), 2u);
    }
    else
    {
        hud_assert_eq(sizeof(wchar), 4u);
    }
    hud_assert_eq(sizeof(f32), 4u);
    hud_assert_eq(sizeof(f64), 8u);

    if constexpr (hud::compilation::is_targeting_64bits())
    {
        hud_assert_eq(sizeof(iptr), 8u);
        hud_assert_eq(sizeof(uptr), 8u);
        hud_assert_eq(sizeof(isize), 8u);
        hud_assert_eq(sizeof(usize), 8u);
    }
    else
    {
        hud_assert_eq(sizeof(iptr), 4u);
        hud_assert_eq(sizeof(uptr), 4u);
        hud_assert_eq(sizeof(isize), 4u);
        hud_assert_eq(sizeof(usize), 4u);
    }
}

GTEST_TEST(types, signed_unsigned)
{
    hud_assert_true(hud::is_unsigned_v<bool>);
    hud_assert_true(hud::is_signed_v<i8>);
    hud_assert_true(hud::is_signed_v<i16>);
    hud_assert_true(hud::is_signed_v<i32>);
    hud_assert_true(hud::is_signed_v<i64>);
    hud_assert_true(hud::is_unsigned_v<u8>);
    hud_assert_true(hud::is_unsigned_v<u16>);
    hud_assert_true(hud::is_unsigned_v<u32>);
    hud_assert_true(hud::is_unsigned_v<u64>);
    hud_assert_true(hud::is_signed_v<ansichar>);
#if defined(HD_OS_WINDOWS)
    hud_assert_true(hud::is_unsigned_v<wchar>);
#else // HD_OS_LINUX
    hud_assert_true(hud::is_signed_v<wchar>);
#endif
    hud_assert_true(hud::is_signed_v<f32>);
    hud_assert_true(hud::is_signed_v<f64>);
    hud_assert_true(hud::is_signed_v<iptr>);
    hud_assert_true(hud::is_unsigned_v<uptr>);
    hud_assert_true(hud::is_signed_v<isize>);
    hud_assert_true(hud::is_unsigned_v<usize>);
}

GTEST_TEST(types, limits)
{
    hud_assert_eq(hud::u8_max, std::numeric_limits<u8>::max());
    hud_assert_eq(hud::u8_min, std::numeric_limits<u8>::min());
    hud_assert_eq(hud::u16_max, std::numeric_limits<u16>::max());
    hud_assert_eq(hud::u16_min, std::numeric_limits<u16>::min());
    hud_assert_eq(hud::u32_max, std::numeric_limits<u32>::max());
    hud_assert_eq(hud::u32_min, std::numeric_limits<u32>::min());
    hud_assert_eq(hud::u64_max, std::numeric_limits<u64>::max());
    hud_assert_eq(hud::u64_min, std::numeric_limits<u64>::min());

    hud_assert_eq(hud::i8_max, std::numeric_limits<i8>::max());
    hud_assert_eq(hud::i8_min, std::numeric_limits<i8>::min());
    hud_assert_eq(hud::i16_max, std::numeric_limits<i16>::max());
    hud_assert_eq(hud::i16_min, std::numeric_limits<i16>::min());
    hud_assert_eq(hud::i32_max, std::numeric_limits<i32>::max());
    hud_assert_eq(hud::i32_min, std::numeric_limits<i32>::min());
    hud_assert_eq(hud::i64_max, std::numeric_limits<i64>::max());
    hud_assert_eq(hud::i64_min, std::numeric_limits<i64>::min());

    hud_assert_eq(hud::ansichar_max, std::numeric_limits<ansichar>::max());
    hud_assert_eq(hud::ansichar_min, std::numeric_limits<ansichar>::min());

    hud_assert_eq(hud::wchar_max, std::numeric_limits<wchar>::max());
    hud_assert_eq(hud::wchar_min, std::numeric_limits<wchar>::min());

    hud_assert_eq(hud::f32_max, std::numeric_limits<f32>::max());
    hud_assert_eq(hud::f32_min, std::numeric_limits<f32>::lowest());
    hud_assert_eq(hud::f32_min_positive, std::numeric_limits<f32>::min());
    hud_assert_eq(hud::f64_max, std::numeric_limits<f64>::max());
    hud_assert_eq(hud::f64_min, std::numeric_limits<f64>::lowest());
    hud_assert_eq(hud::f64_min_positive, std::numeric_limits<f64>::min());

    hud_assert_eq(hud::uptr_max, std::numeric_limits<uptr>::max());
    hud_assert_eq(hud::uptr_min, std::numeric_limits<uptr>::min());
    hud_assert_eq(hud::iptr_max, std::numeric_limits<iptr>::max());
    hud_assert_eq(hud::iptr_min, std::numeric_limits<iptr>::min());

    hud_assert_eq(hud::usize_max, std::numeric_limits<usize>::max());
    hud_assert_eq(hud::usize_min, std::numeric_limits<usize>::min());
    hud_assert_eq(hud::isize_max, std::numeric_limits<isize>::max());
    hud_assert_eq(hud::isize_min, std::numeric_limits<isize>::min());
}