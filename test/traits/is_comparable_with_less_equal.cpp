#include <core/traits/is_comparable_with_less_equal.h>
namespace hud_test
{
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

    struct a
    {
    };
    struct b
    {
        bool operator<=(const b &) const noexcept;
    };
    struct c
    {
        bool operator<=(const a &) const noexcept;
    };
}

GTEST_TEST(traits, is_comparable_with_less_equal)
{

    // i8
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i8, char32>));

    // u8
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u8, char32>));

    // i16
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i16, char32>));

    // u16
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u16, char32>));

    // i32
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i32, char32>));

    // u32
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u32, char32>));

    // i64
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<i64, char32>));

    // u64
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<u64, char32>));

    // iptr
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<iptr, char32>));

    // uptr
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<uptr, char32>));

    // f32
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f32, char32>));

    // f64
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<f64, char32>));

    // ansichar
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<ansichar, char32>));

    // wchar
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<wchar, char32>));

    // char16
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char16, char32>));

    // char32
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, i8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, i16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, i32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, i64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, iptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, u8>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, u16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, u32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, u64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, uptr>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, f32>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, f64>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, ansichar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, wchar>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, char16>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<char32, char32>));

    // enum
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum, hud_test::comp_enum>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum, hud_test::comp_enum_2>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum, hud_test::comp_enum_3>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum, hud_test::comp_enum_4>));

    // enum class
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_2, hud_test::comp_enum>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_2, hud_test::comp_enum_2>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_2, hud_test::comp_enum_3>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_2, hud_test::comp_enum_4>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_3, hud_test::comp_enum>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_3, hud_test::comp_enum_2>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_3, hud_test::comp_enum_3>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_3, hud_test::comp_enum_4>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_4, hud_test::comp_enum>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_4, hud_test::comp_enum_2>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_4, hud_test::comp_enum_3>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<hud_test::comp_enum_4, hud_test::comp_enum_4>));

    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::a, hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::a, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::a, hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::b, hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<hud_test::b, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::b, hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<hud_test::c, hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::c, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<hud_test::c, hud_test::c>));

    // pointers
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, void>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, i8>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, i16>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, i32>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, i64>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, iptr>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, u8>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, u16>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, u32>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, u64>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, uptr>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, f32>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, f64>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, ansichar>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, wchar>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, char16>));
    GTEST_ASSERT_FALSE((hud::is_comparable_with_less_equal_v<void *, char32>));

    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, hud_test::a *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, hud_test::b *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, hud_test::c *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, void *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, i8 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, i16 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, i32 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, i64 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, iptr *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, u8 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, u16 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, u32 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, u64 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, uptr *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, f32 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, f64 *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, ansichar *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, wchar *>));
    GTEST_ASSERT_TRUE((hud::is_comparable_with_less_equal_v<void *, char16 *>));
}