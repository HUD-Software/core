#include <core/traits/is_bitwise_move_constructible.h>

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
        b(b &&) noexcept
        {
        }
    };

    struct c
    {
        virtual void fn()
        {
        }
    };

    struct d
    {
    };

} // namespace hud_test

GTEST_TEST(traits, is_bitwise_move_constructible)
{
    // i8
    hud_assert_true((hud::is_bitwise_move_constructible_v<i8, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, i64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i8, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, u64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i8, ansichar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, wchar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i8, uptr>));

    // u8
    hud_assert_true((hud::is_bitwise_move_constructible_v<u8, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, i64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u8, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, u64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u8, ansichar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, wchar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u8, uptr>));

    // i16
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, i8>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i16, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, u8>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i16, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, ansichar>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<i16, wchar>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, wchar>));
#endif
    hud_assert_true((hud::is_bitwise_move_constructible_v<i16, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i16, uptr>));

    // u16
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, i8>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u16, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, u8>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u16, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, ansichar>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<u16, wchar>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, wchar>));
#endif
    hud_assert_true((hud::is_bitwise_move_constructible_v<u16, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u16, uptr>));

    // i32
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, i16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, u16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, ansichar>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, wchar>));
#else
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32, wchar>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, char16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, f64>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32, iptr>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32, uptr>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32, uptr>));
#endif

    // u32
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, i16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u32, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, u16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u32, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, ansichar>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, wchar>));
#else
    hud_assert_true((hud::is_bitwise_move_constructible_v<u32, wchar>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, char16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u32, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, f64>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<u32, iptr>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u32, uptr>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u32, uptr>));
#endif

    // i64
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, i32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i64, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, u32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i64, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, ansichar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, wchar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, f64>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i64, uptr>));
#else
    hud_assert_true((hud::is_bitwise_move_constructible_v<i64, iptr>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i64, uptr>));
#endif

    // u64
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, i32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u64, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, u32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u64, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, ansichar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, wchar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, f64>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<u64, uptr>));
#else
    hud_assert_true((hud::is_bitwise_move_constructible_v<u64, iptr>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<u64, uptr>));
#endif

    // ansichar
    hud_assert_true((hud::is_bitwise_move_constructible_v<ansichar, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, i64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<ansichar, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, u64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<ansichar, ansichar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, wchar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<ansichar, uptr>));

    // wchar
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, i8>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, i32>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, i16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, i32>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, u8>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, u32>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, u16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, u32>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, ansichar>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, wchar>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, char32>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, char16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, char32>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, f64>));
    if (sizeof(wchar) == sizeof(iptr))
    {
        hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, iptr>));
    }
    else
    {
        hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, iptr>));
    }
    if (sizeof(wchar) == sizeof(uptr))
    {
        hud_assert_true((hud::is_bitwise_move_constructible_v<wchar, uptr>));
    }
    else
    {
        hud_assert_false((hud::is_bitwise_move_constructible_v<wchar, uptr>));
    }

    // char16
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, i8>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<char16, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, u8>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<char16, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, ansichar>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<char16, wchar>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, wchar>));
#endif
    hud_assert_true((hud::is_bitwise_move_constructible_v<char16, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char16, uptr>));

    // char32
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, i16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<char32, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, u16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<char32, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, ansichar>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, wchar>));
#else
    hud_assert_true((hud::is_bitwise_move_constructible_v<char32, wchar>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, char16>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<char32, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, f64>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<char32, iptr>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<char32, uptr>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<char32, uptr>));
#endif
    // f32
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, ansichar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, wchar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, char32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<f32, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f32, uptr>));

    // f64
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, i16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, i64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, u16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, u64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, ansichar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, wchar>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, char16>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, char32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, f32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<f64, f64>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, iptr>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<f64, uptr>));

    // iptr
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, i16>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, i64>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, i32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, i64>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, u16>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, u64>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, u32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, u64>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, ansichar>));
    if (sizeof(wchar) == sizeof(iptr))
    {
        hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, wchar>));
    }
    else
    {
        hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, wchar>));
    }
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, char16>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, char32>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, char32>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<iptr, f64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, iptr>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<iptr, uptr>));

    // uptr
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, i8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, i16>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, i32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, i64>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, i32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, i64>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, u8>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, u16>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, u32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, u64>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, u32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, u64>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, ansichar>));
    if (sizeof(wchar) == sizeof(uptr))
    {
        hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, wchar>));
    }
    else
    {
        hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, wchar>));
    }
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, char16>));
#if defined(HD_TARGET_32_BITS)
    hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, char32>));
#else
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, char32>));
#endif
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, f32>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<uptr, f64>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, iptr>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<uptr, uptr>));

    hud_assert_true((hud::is_bitwise_move_constructible_v<hud_test::comp_enum, hud_test::comp_enum>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<hud_test::comp_enum_2, hud_test::comp_enum_2>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<hud_test::comp_enum_3, hud_test::comp_enum_3>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<hud_test::comp_enum_4, hud_test::comp_enum_4>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<hud_test::comp_enum, hud_test::comp_enum_2>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<hud_test::comp_enum, hud_test::comp_enum_3>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<hud_test::comp_enum, hud_test::comp_enum_4>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<hud_test::comp_enum_3, hud_test::comp_enum_4>));

    hud_assert_true((hud::is_bitwise_move_constructible_v<i32, i32>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32 &, i32 &>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32[], i32[]>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<i32[32], i32[32]>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32 *, i32 *>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32 *, u32 *>));
    hud_assert_true((hud::is_bitwise_move_constructible_v<i32 *, u8 *>));

    hud_assert_true((hud::is_bitwise_move_constructible_v<hud_test::a, hud_test::a>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<hud_test::b, hud_test::b>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<hud_test::c, hud_test::c>));
    hud_assert_false((hud::is_bitwise_move_constructible_v<hud_test::a, hud_test::d>));
}