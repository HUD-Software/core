#include <core/traits/is_signed.h>

namespace hud_test
{
    struct a
    {
    };
    enum class e : i32
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_signed)
{
    hud_assert_false((hud::is_signed_v<bool>));
    hud_assert_true((hud::is_signed_v<i8>));
    hud_assert_true((hud::is_signed_v<i16>));
    hud_assert_true((hud::is_signed_v<i32>));
    hud_assert_true((hud::is_signed_v<i64>));
    hud_assert_true((hud::is_signed_v<iptr>));
    hud_assert_true((hud::is_signed_v<f32>));
    hud_assert_true((hud::is_signed_v<f64>));

    hud_assert_false((hud::is_signed_v<u8>));
    hud_assert_false((hud::is_signed_v<u16>));
    hud_assert_false((hud::is_signed_v<u32>));
    hud_assert_false((hud::is_signed_v<u64>));
    hud_assert_false((hud::is_signed_v<uptr>));

    hud_assert_true((hud::is_signed_v<isize>));
    hud_assert_false((hud::is_signed_v<usize>));
    hud_assert_false((hud::is_signed_v<char16>));
    hud_assert_false((hud::is_signed_v<char32>));
    hud_assert_true((hud::is_signed_v<char8>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_false((hud::is_signed_v<wchar>));
#else // HD_HOST_LINUX
    hud_assert_true((hud::is_signed_v<wchar>));
#endif
    hud_assert_false((hud::is_signed_v<hud_test::a>));

    hud_assert_false((hud::is_signed_v<const bool>));
    hud_assert_true((hud::is_signed_v<const i8>));
    hud_assert_true((hud::is_signed_v<const i16>));
    hud_assert_true((hud::is_signed_v<const i32>));
    hud_assert_true((hud::is_signed_v<const i64>));
    hud_assert_true((hud::is_signed_v<const iptr>));
    hud_assert_true((hud::is_signed_v<const f32>));
    hud_assert_true((hud::is_signed_v<const f64>));

    hud_assert_false((hud::is_signed_v<const u8>));
    hud_assert_false((hud::is_signed_v<const u16>));
    hud_assert_false((hud::is_signed_v<const u32>));
    hud_assert_false((hud::is_signed_v<const u64>));
    hud_assert_false((hud::is_signed_v<const uptr>));

    hud_assert_true((hud::is_signed_v<const isize>));
    hud_assert_false((hud::is_signed_v<const usize>));
    hud_assert_false((hud::is_signed_v<const char16>));
    hud_assert_false((hud::is_signed_v<const char32>));
    hud_assert_true((hud::is_signed_v<const char8>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_false((hud::is_signed_v<const wchar>));
#else // HD_HOST_LINUX
    hud_assert_true((hud::is_signed_v<const wchar>));
#endif
    hud_assert_false((hud::is_signed_v<const hud_test::a>));

    hud_assert_false((hud::is_signed_v<volatile bool>));
    hud_assert_true((hud::is_signed_v<volatile i8>));
    hud_assert_true((hud::is_signed_v<volatile i16>));
    hud_assert_true((hud::is_signed_v<volatile i32>));
    hud_assert_true((hud::is_signed_v<volatile i64>));
    hud_assert_true((hud::is_signed_v<volatile iptr>));
    hud_assert_true((hud::is_signed_v<volatile f32>));
    hud_assert_true((hud::is_signed_v<volatile f64>));

    hud_assert_false((hud::is_signed_v<volatile u8>));
    hud_assert_false((hud::is_signed_v<volatile u16>));
    hud_assert_false((hud::is_signed_v<volatile u32>));
    hud_assert_false((hud::is_signed_v<volatile u64>));
    hud_assert_false((hud::is_signed_v<volatile uptr>));

    hud_assert_true((hud::is_signed_v<volatile isize>));
    hud_assert_false((hud::is_signed_v<volatile usize>));
    hud_assert_false((hud::is_signed_v<volatile char16>));
    hud_assert_false((hud::is_signed_v<volatile char32>));
    hud_assert_true((hud::is_signed_v<volatile char8>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_false((hud::is_signed_v<volatile wchar>));
#else // HD_HOST_LINUX
    hud_assert_true((hud::is_signed_v<volatile wchar>));
#endif
    hud_assert_false((hud::is_signed_v<volatile hud_test::a>));

    hud_assert_false((hud::is_signed_v<const volatile bool>));
    hud_assert_true((hud::is_signed_v<const volatile i8>));
    hud_assert_true((hud::is_signed_v<const volatile i16>));
    hud_assert_true((hud::is_signed_v<const volatile i32>));
    hud_assert_true((hud::is_signed_v<const volatile i64>));
    hud_assert_true((hud::is_signed_v<const volatile iptr>));
    hud_assert_true((hud::is_signed_v<const volatile f32>));
    hud_assert_true((hud::is_signed_v<const volatile f64>));

    hud_assert_false((hud::is_signed_v<const volatile u8>));
    hud_assert_false((hud::is_signed_v<const volatile u16>));
    hud_assert_false((hud::is_signed_v<const volatile u32>));
    hud_assert_false((hud::is_signed_v<const volatile u64>));
    hud_assert_false((hud::is_signed_v<const volatile uptr>));

    hud_assert_true((hud::is_signed_v<const volatile isize>));
    hud_assert_false((hud::is_signed_v<const volatile usize>));
    hud_assert_false((hud::is_signed_v<const volatile char16>));
    hud_assert_false((hud::is_signed_v<const volatile char32>));
    hud_assert_true((hud::is_signed_v<const volatile char8>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_false((hud::is_signed_v<const volatile wchar>));
#else // HD_HOST_LINUX
    hud_assert_true((hud::is_signed_v<const volatile wchar>));
#endif
    hud_assert_false((hud::is_signed_v<const volatile hud_test::a>));

    hud_assert_false((hud::is_signed_v<hud_test::e>));
}
