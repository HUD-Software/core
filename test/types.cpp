#include <core/types.h>
#include <core/traits/is_signed.h>
#include <core/traits/is_unsigned.h>

GTEST_TEST(types, size)
{
    GTEST_ASSERT_EQ(sizeof(bool), 1u);
    GTEST_ASSERT_EQ(sizeof(i8), 1u);
    GTEST_ASSERT_EQ(sizeof(i16), 2u);
    GTEST_ASSERT_EQ(sizeof(i32), 4u);
    GTEST_ASSERT_EQ(sizeof(i64), 8u);
    GTEST_ASSERT_EQ(sizeof(u8), 1u);
    GTEST_ASSERT_EQ(sizeof(u16), 2u);
    GTEST_ASSERT_EQ(sizeof(u32), 4u);
    GTEST_ASSERT_EQ(sizeof(u64), 8u);
    GTEST_ASSERT_EQ(sizeof(ansichar), 1u);

    if constexpr (hud::compilation::is_os(hud::os_e::windows))
    {
        GTEST_ASSERT_EQ(sizeof(wchar), 2u);
    }
    else
    {
        GTEST_ASSERT_EQ(sizeof(wchar), 4u);
    }
    GTEST_ASSERT_EQ(sizeof(f32), 4u);
    GTEST_ASSERT_EQ(sizeof(f64), 8u);

    if constexpr (hud::compilation::is_targeting_64bits())
    {
        GTEST_ASSERT_EQ(sizeof(iptr), 8u);
        GTEST_ASSERT_EQ(sizeof(uptr), 8u);
        GTEST_ASSERT_EQ(sizeof(isize), 8u);
        GTEST_ASSERT_EQ(sizeof(usize), 8u);
    }
    else
    {
        GTEST_ASSERT_EQ(sizeof(iptr), 4u);
        GTEST_ASSERT_EQ(sizeof(uptr), 4u);
        GTEST_ASSERT_EQ(sizeof(isize), 4u);
        GTEST_ASSERT_EQ(sizeof(usize), 4u);
    }
}

GTEST_TEST(types, signed_unsigned)
{
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<bool>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<i8>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<i16>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<i32>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<i64>);
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<u8>);
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<u16>);
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<u32>);
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<u64>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<ansichar>);
#if defined(HD_OS_WINDOWS)
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<wchar>);
#else // HD_OS_LINUX
    GTEST_ASSERT_TRUE(hud::is_signed_v<wchar>);
#endif
    GTEST_ASSERT_TRUE(hud::is_signed_v<f32>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<f64>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<iptr>);
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<uptr>);
    GTEST_ASSERT_TRUE(hud::is_signed_v<isize>);
    GTEST_ASSERT_TRUE(hud::is_unsigned_v<usize>);
}

GTEST_TEST(types, limits)
{
    GTEST_ASSERT_EQ(hud::u8_max, std::numeric_limits<u8>::max());
    GTEST_ASSERT_EQ(hud::u8_min, std::numeric_limits<u8>::min());
    GTEST_ASSERT_EQ(hud::u16_max, std::numeric_limits<u16>::max());
    GTEST_ASSERT_EQ(hud::u16_min, std::numeric_limits<u16>::min());
    GTEST_ASSERT_EQ(hud::u32_max, std::numeric_limits<u32>::max());
    GTEST_ASSERT_EQ(hud::u32_min, std::numeric_limits<u32>::min());
    GTEST_ASSERT_EQ(hud::u64_max, std::numeric_limits<u64>::max());
    GTEST_ASSERT_EQ(hud::u64_min, std::numeric_limits<u64>::min());

    GTEST_ASSERT_EQ(hud::i8_max, std::numeric_limits<i8>::max());
    GTEST_ASSERT_EQ(hud::i8_min, std::numeric_limits<i8>::min());
    GTEST_ASSERT_EQ(hud::i16_max, std::numeric_limits<i16>::max());
    GTEST_ASSERT_EQ(hud::i16_min, std::numeric_limits<i16>::min());
    GTEST_ASSERT_EQ(hud::i32_max, std::numeric_limits<i32>::max());
    GTEST_ASSERT_EQ(hud::i32_min, std::numeric_limits<i32>::min());
    GTEST_ASSERT_EQ(hud::i64_max, std::numeric_limits<i64>::max());
    GTEST_ASSERT_EQ(hud::i64_min, std::numeric_limits<i64>::min());

    GTEST_ASSERT_EQ(hud::ansichar_max, std::numeric_limits<ansichar>::max());
    GTEST_ASSERT_EQ(hud::ansichar_min, std::numeric_limits<ansichar>::min());

    GTEST_ASSERT_EQ(hud::wchar_max, std::numeric_limits<wchar>::max());
    GTEST_ASSERT_EQ(hud::wchar_min, std::numeric_limits<wchar>::min());

    GTEST_ASSERT_EQ(hud::f32_max, std::numeric_limits<f32>::max());
    GTEST_ASSERT_EQ(hud::f32_min, std::numeric_limits<f32>::min());
    GTEST_ASSERT_EQ(hud::f64_max, std::numeric_limits<f64>::max());
    GTEST_ASSERT_EQ(hud::f64_min, std::numeric_limits<f64>::min());

    GTEST_ASSERT_EQ(hud::uptr_max, std::numeric_limits<uptr>::max());
    GTEST_ASSERT_EQ(hud::uptr_min, std::numeric_limits<uptr>::min());
    GTEST_ASSERT_EQ(hud::iptr_max, std::numeric_limits<iptr>::max());
    GTEST_ASSERT_EQ(hud::iptr_min, std::numeric_limits<iptr>::min());

    GTEST_ASSERT_EQ(hud::usize_max, std::numeric_limits<usize>::max());
    GTEST_ASSERT_EQ(hud::usize_min, std::numeric_limits<usize>::min());
    GTEST_ASSERT_EQ(hud::isize_max, std::numeric_limits<isize>::max());
    GTEST_ASSERT_EQ(hud::isize_min, std::numeric_limits<isize>::min());
}