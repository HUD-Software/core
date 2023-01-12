#include <core/traits/is_signed.h>

namespace hud_test
{
    struct a
    {
    };
    enum class e : i32
    {
    };
}

GTEST_TEST(traits, is_signed)
{
    GTEST_ASSERT_FALSE((hud::is_signed_v<bool>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<i8>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<i16>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<i64>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<iptr>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<f32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<f64>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<u8>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<u16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<u32>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<u64>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<uptr>));

    GTEST_ASSERT_TRUE((hud::is_signed_v<isize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<usize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<char16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<char32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<ansichar>));
#if defined(HD_OS_WINDOWS)
    GTEST_ASSERT_FALSE((hud::is_signed_v<wchar>));
#else // HD_OS_LINUX
    GTEST_ASSERT_TRUE((hud::is_signed_v<wchar>));
#endif
    GTEST_ASSERT_FALSE((hud::is_signed_v<hud_test::a>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<const bool>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const i8>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const i16>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const i32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const i64>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const iptr>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const f32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const f64>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<const u8>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const u16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const u32>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const u64>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const uptr>));

    GTEST_ASSERT_TRUE((hud::is_signed_v<const isize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const usize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const char16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const char32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const ansichar>));
#if defined(HD_OS_WINDOWS)
    GTEST_ASSERT_FALSE((hud::is_signed_v<const wchar>));
#else // HD_OS_LINUX
    GTEST_ASSERT_TRUE((hud::is_signed_v<const wchar>));
#endif
    GTEST_ASSERT_FALSE((hud::is_signed_v<const hud_test::a>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile bool>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile i8>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile i16>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile iptr>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile f32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile f64>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile u8>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile u16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile u32>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile u64>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile uptr>));

    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile isize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile usize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile char16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile char32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile ansichar>));
#if defined(HD_OS_WINDOWS)
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile wchar>));
#else // HD_OS_LINUX
    GTEST_ASSERT_TRUE((hud::is_signed_v<volatile wchar>));
#endif
    GTEST_ASSERT_FALSE((hud::is_signed_v<volatile hud_test::a>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile bool>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile i8>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile i16>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile iptr>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile f32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile f64>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile u8>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile u16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile u32>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile u64>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile uptr>));

    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile isize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile usize>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile char16>));
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile char32>));
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile ansichar>));
#if defined(HD_OS_WINDOWS)
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile wchar>));
#else // HD_OS_LINUX
    GTEST_ASSERT_TRUE((hud::is_signed_v<const volatile wchar>));
#endif
    GTEST_ASSERT_FALSE((hud::is_signed_v<const volatile hud_test::a>));

    GTEST_ASSERT_FALSE((hud::is_signed_v<hud_test::e>));
}
