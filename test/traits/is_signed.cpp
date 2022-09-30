#include <core/traits/is_signed.h>

namespace hud_test {
    struct a {};
    enum class e : i32 {};
}

TEST(traits, is_signed) {
    ASSERT_FALSE((hud::is_signed_v<bool>));
    ASSERT_TRUE((hud::is_signed_v<i8>));
    ASSERT_TRUE((hud::is_signed_v<i16>));
    ASSERT_TRUE((hud::is_signed_v<i32>));
    ASSERT_TRUE((hud::is_signed_v<i64>));
    ASSERT_TRUE((hud::is_signed_v<iptr>));
    ASSERT_TRUE((hud::is_signed_v<f32>));
    ASSERT_TRUE((hud::is_signed_v<f64>));

    ASSERT_FALSE((hud::is_signed_v<u8>));
    ASSERT_FALSE((hud::is_signed_v<u16>));
    ASSERT_FALSE((hud::is_signed_v<u32>));
    ASSERT_FALSE((hud::is_signed_v<u64>));
    ASSERT_FALSE((hud::is_signed_v<uptr>));

    ASSERT_TRUE((hud::is_signed_v<isize>));
    ASSERT_FALSE((hud::is_signed_v<usize>));
    ASSERT_FALSE((hud::is_signed_v<char16>));
    ASSERT_FALSE((hud::is_signed_v<char32>));
    ASSERT_TRUE((hud::is_signed_v<ansichar>));
    #if defined(HD_OS_WINDOWS)
    ASSERT_FALSE((hud::is_signed_v<wchar>));
    #else // HD_OS_LINUX
    ASSERT_TRUE((hud::is_signed_v<wchar>));
    #endif
    ASSERT_FALSE((hud::is_signed_v<hud_test::a>));

    ASSERT_FALSE((hud::is_signed_v<const bool>));
    ASSERT_TRUE((hud::is_signed_v<const i8>));
    ASSERT_TRUE((hud::is_signed_v<const i16>));
    ASSERT_TRUE((hud::is_signed_v<const i32>));
    ASSERT_TRUE((hud::is_signed_v<const i64>));
    ASSERT_TRUE((hud::is_signed_v<const iptr>));
    ASSERT_TRUE((hud::is_signed_v<const f32>));
    ASSERT_TRUE((hud::is_signed_v<const f64>));

    ASSERT_FALSE((hud::is_signed_v<const u8>));
    ASSERT_FALSE((hud::is_signed_v<const u16>));
    ASSERT_FALSE((hud::is_signed_v<const u32>));
    ASSERT_FALSE((hud::is_signed_v<const u64>));
    ASSERT_FALSE((hud::is_signed_v<const uptr>));

    ASSERT_TRUE((hud::is_signed_v<const isize>));
    ASSERT_FALSE((hud::is_signed_v<const usize>));
    ASSERT_FALSE((hud::is_signed_v<const char16>));
    ASSERT_FALSE((hud::is_signed_v<const char32>));
    ASSERT_TRUE((hud::is_signed_v<const ansichar>));
    #if defined(HD_OS_WINDOWS)
    ASSERT_FALSE((hud::is_signed_v<const wchar>));
    #else // HD_OS_LINUX
    ASSERT_TRUE((hud::is_signed_v<const wchar>));
    #endif
    ASSERT_FALSE((hud::is_signed_v<const hud_test::a>));

    ASSERT_FALSE((hud::is_signed_v<volatile bool>));
    ASSERT_TRUE((hud::is_signed_v<volatile i8>));
    ASSERT_TRUE((hud::is_signed_v<volatile i16>));
    ASSERT_TRUE((hud::is_signed_v<volatile i32>));
    ASSERT_TRUE((hud::is_signed_v<volatile i64>));
    ASSERT_TRUE((hud::is_signed_v<volatile iptr>));
    ASSERT_TRUE((hud::is_signed_v<volatile f32>));
    ASSERT_TRUE((hud::is_signed_v<volatile f64>));

    ASSERT_FALSE((hud::is_signed_v<volatile u8>));
    ASSERT_FALSE((hud::is_signed_v<volatile u16>));
    ASSERT_FALSE((hud::is_signed_v<volatile u32>));
    ASSERT_FALSE((hud::is_signed_v<volatile u64>));
    ASSERT_FALSE((hud::is_signed_v<volatile uptr>));

    ASSERT_TRUE((hud::is_signed_v<volatile isize>));
    ASSERT_FALSE((hud::is_signed_v<volatile usize>));
    ASSERT_FALSE((hud::is_signed_v<volatile char16>));
    ASSERT_FALSE((hud::is_signed_v<volatile char32>));
    ASSERT_TRUE((hud::is_signed_v<volatile ansichar>));
    #if defined(HD_OS_WINDOWS)
    ASSERT_FALSE((hud::is_signed_v<volatile wchar>));
    #else // HD_OS_LINUX
    ASSERT_TRUE((hud::is_signed_v<volatile wchar>));
    #endif
    ASSERT_FALSE((hud::is_signed_v<volatile hud_test::a>));

    ASSERT_FALSE((hud::is_signed_v<const volatile bool>));
    ASSERT_TRUE((hud::is_signed_v<const volatile i8>));
    ASSERT_TRUE((hud::is_signed_v<const volatile i16>));
    ASSERT_TRUE((hud::is_signed_v<const volatile i32>));
    ASSERT_TRUE((hud::is_signed_v<const volatile i64>));
    ASSERT_TRUE((hud::is_signed_v<const volatile iptr>));
    ASSERT_TRUE((hud::is_signed_v<const volatile f32>));
    ASSERT_TRUE((hud::is_signed_v<const volatile f64>));

    ASSERT_FALSE((hud::is_signed_v<const volatile u8>));
    ASSERT_FALSE((hud::is_signed_v<const volatile u16>));
    ASSERT_FALSE((hud::is_signed_v<const volatile u32>));
    ASSERT_FALSE((hud::is_signed_v<const volatile u64>));
    ASSERT_FALSE((hud::is_signed_v<const volatile uptr>));

    ASSERT_TRUE((hud::is_signed_v<const volatile isize>));
    ASSERT_FALSE((hud::is_signed_v<const volatile usize>));
    ASSERT_FALSE((hud::is_signed_v<const volatile char16>));
    ASSERT_FALSE((hud::is_signed_v<const volatile char32>));
    ASSERT_TRUE((hud::is_signed_v<const volatile ansichar>));
    #if defined(HD_OS_WINDOWS)
    ASSERT_FALSE((hud::is_signed_v<const volatile wchar>));
    #else // HD_OS_LINUX
    ASSERT_TRUE((hud::is_signed_v<const volatile wchar>));
    #endif
    ASSERT_FALSE((hud::is_signed_v<const volatile hud_test::a>));

    ASSERT_FALSE((hud::is_signed_v<hud_test::e>));
}
