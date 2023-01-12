#include <core/traits/is_bitwise_comparable.h>

namespace hud_test
{
    struct empty
    {
    };
    struct a
    {
        i32 a;
    };
    struct b
    {
        i32 a;
        f32 f;
    };
    struct c
    {
        i32 a;
        i16 f;
        i16 g;
    };
    struct padded
    {
        u8 p;
        // Padding of 3 bytes
        u32 a;
    };
    struct padded_2
    {
        u32 p;
        u16 a;
        // Padding of 2 bytes
    };
    struct derived : a
    {
    };
    struct derived_2 : a
    {
        virtual void fn() {}
    };
    struct d
    {
        bool operator==(const d &) { return true; }
    };

    struct f
    {
        i32 a;
        short b;
        bool operator==(const f &) { return true; }
    };

    struct comp_e
    {
        i32 a;
        bool operator==(const comp_e &) { return true; }
    };

    enum comp_enum
    {
    };
    enum class comp_enum_2
    {
    };
    enum class comp_enum_3 : u8
    {
    };
    enum class comp_enum_4 : u64
    {
    };
}

GTEST_TEST(traits, is_bitwise_comparable)
{
    // i8
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i8, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, i64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i8, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, u64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i8, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i8, uptr>));

    // u8
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u8, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, i64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u8, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, u64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u8, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u8, uptr>));

    // i16
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, i8>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i16, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, u8>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i16, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, ansichar>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i16, wchar>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, wchar>));
#endif
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i16, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i16, uptr>));

    // u16
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, i8>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u16, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, u8>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u16, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, ansichar>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u16, wchar>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, wchar>));
#endif
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u16, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u16, uptr>));

    // i32
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, i16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i32, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, u16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i32, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, ansichar>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, wchar>));
#else
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i32, wchar>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, char16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i32, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, f64>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i32, iptr>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i32, uptr>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i32, uptr>));
#endif

    // u32
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, i16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u32, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, u16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u32, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, ansichar>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, wchar>));
#else
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u32, wchar>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, char16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u32, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, f64>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u32, iptr>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u32, uptr>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u32, uptr>));
#endif

    // i64
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, i32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i64, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, u32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i64, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, f64>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<i64, uptr>));
#else
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i64, iptr>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<i64, uptr>));
#endif

    // u64
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, i32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u64, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, u32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u64, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, f64>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<u64, uptr>));
#else
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u64, iptr>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<u64, uptr>));
#endif

    // ansichar
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<ansichar, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, i64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<ansichar, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, u64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<ansichar, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<ansichar, uptr>));

    // wchar
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, i8>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<wchar, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, i32>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, i16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<wchar, i32>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, u8>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<wchar, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, u32>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, u16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<wchar, u32>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<wchar, wchar>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<wchar, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, char32>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, char16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<wchar, char32>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<wchar, uptr>));

    // char16
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, i8>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char16, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, u8>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char16, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, ansichar>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char16, wchar>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, wchar>));
#endif
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char16, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char16, uptr>));

    // char32
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, i16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char32, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, u16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char32, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, ansichar>));
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, wchar>));
#else
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char32, wchar>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, char16>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char32, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, f64>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char32, iptr>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<char32, uptr>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<char32, uptr>));
#endif
    // f32
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f32, uptr>));

    // f64
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, i16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, i64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, u16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, u64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, char16>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, char32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, f64>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, iptr>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<f64, uptr>));

    // iptr
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, i16>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<iptr, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, i64>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, i32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<iptr, i64>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, u16>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<iptr, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, u64>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, u32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<iptr, u64>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, char16>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<iptr, char32>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, char32>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<iptr, f64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<iptr, iptr>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<iptr, uptr>));

    // uptr
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, i8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, i16>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<uptr, i32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, i64>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, i32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<uptr, i64>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, u8>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, u16>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<uptr, u32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, u64>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, u32>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<uptr, u64>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, wchar>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, char16>));
#if defined(HD_TARGET_32_BITS)
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<uptr, char32>));
#else
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, char32>));
#endif
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, f32>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<uptr, f64>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<uptr, iptr>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<uptr, uptr>));

    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::empty, hud_test::empty>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::a, hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::b, hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::c, hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::padded, hud_test::padded>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::padded_2, hud_test::padded_2>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::derived, hud_test::derived>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::derived_2, hud_test::derived_2>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::d, hud_test::d>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::comp_e, hud_test::comp_e>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::f, hud_test::f>));

    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::comp_enum, hud_test::comp_enum>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::comp_enum_2, hud_test::comp_enum_2>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::comp_enum_3, hud_test::comp_enum_3>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::comp_enum_4, hud_test::comp_enum_4>));
    GTEST_ASSERT_TRUE((hud::is_bitwise_comparable_v<hud_test::comp_enum, hud_test::comp_enum_2>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::comp_enum, hud_test::comp_enum_3>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::comp_enum, hud_test::comp_enum_4>));
    GTEST_ASSERT_FALSE((hud::is_bitwise_comparable_v<hud_test::comp_enum_3, hud_test::comp_enum_4>));
}