#include <core/traits/is_comparable_with_less_operator.h>

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
        bool operator<(const b &) const noexcept;
    };

    struct c
    {
        bool operator<(const a &) const noexcept;
    };

} // namespace hud_test

GTEST_TEST(traits, is_comparable_with_less_operator)
{

    // i8
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i8, char32>));

    // u8
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u8, char32>));

    // i16
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i16, char32>));

    // u16
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u16, char32>));

    // i32
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i32, char32>));

    // u32
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u32, char32>));

    // i64
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<i64, char32>));

    // u64
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<u64, char32>));

    // iptr
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<iptr, char32>));

    // uptr
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<uptr, char32>));

    // f32
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f32, char32>));

    // f64
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<f64, char32>));

    // ansichar
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<ansichar, char32>));

    // wchar
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<wchar, char32>));

    // char16
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char16, char32>));

    // char32
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, i8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, i16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, i32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, i64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, iptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, u8>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, u16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, u32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, u64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, uptr>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, f32>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, f64>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, ansichar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, wchar>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, char16>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<char32, char32>));

    // enum
    hud_assert_true((hud::is_comparable_with_less_operator_v<hud_test::comp_enum, hud_test::comp_enum>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum, hud_test::comp_enum_2>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum, hud_test::comp_enum_3>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum, hud_test::comp_enum_4>));

    // enum class
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_2, hud_test::comp_enum>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_2, hud_test::comp_enum_2>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_2, hud_test::comp_enum_3>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_2, hud_test::comp_enum_4>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_3, hud_test::comp_enum>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_3, hud_test::comp_enum_2>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_3, hud_test::comp_enum_3>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_3, hud_test::comp_enum_4>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_4, hud_test::comp_enum>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_4, hud_test::comp_enum_2>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_4, hud_test::comp_enum_3>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<hud_test::comp_enum_4, hud_test::comp_enum_4>));

    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::a, hud_test::a>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::a, hud_test::b>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::a, hud_test::c>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::b, hud_test::a>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<hud_test::b, hud_test::b>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::b, hud_test::c>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<hud_test::c, hud_test::a>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::c, hud_test::b>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<hud_test::c, hud_test::c>));

    // pointers
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, hud_test::a>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, hud_test::b>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, hud_test::c>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, void>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, i8>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, i16>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, i32>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, i64>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, iptr>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, u8>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, u16>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, u32>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, u64>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, uptr>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, f32>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, f64>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, ansichar>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, wchar>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, char16>));
    hud_assert_false((hud::is_comparable_with_less_operator_v<void *, char32>));

    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, hud_test::a *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, hud_test::b *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, hud_test::c *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, void *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, i8 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, i16 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, i32 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, i64 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, iptr *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, u8 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, u16 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, u32 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, u64 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, uptr *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, f32 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, f64 *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, ansichar *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, wchar *>));
    hud_assert_true((hud::is_comparable_with_less_operator_v<void *, char16 *>));
}