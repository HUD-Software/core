#include <core/types.h>
#include <core/traits/is_signed.h>
#include <core/traits/is_unsigned.h>

TEST(Types, size) {
    ASSERT_EQ(sizeof(bool), 1u);
    ASSERT_EQ(sizeof(i8), 1u);
    ASSERT_EQ(sizeof(i16), 2u);
    ASSERT_EQ(sizeof(i32), 4u);
    ASSERT_EQ(sizeof(i64), 8u);
    ASSERT_EQ(sizeof(u8), 1u);
    ASSERT_EQ(sizeof(u16), 2u);
    ASSERT_EQ(sizeof(u32), 4u);
    ASSERT_EQ(sizeof(u64), 8u);
    ASSERT_EQ(sizeof(ansichar), 1u);

    if constexpr (hud::compilation::is_os(hud::os_e::windows)) {
        ASSERT_EQ(sizeof(wchar), 2u);
    }
    else {
        ASSERT_EQ(sizeof(wchar), 4u);
    }
    ASSERT_EQ(sizeof(f32), 4u);
    ASSERT_EQ(sizeof(f64), 8u);

    if constexpr (hud::compilation::is_targeting_64bits()) {
        ASSERT_EQ(sizeof(iptr), 8u);
        ASSERT_EQ(sizeof(uptr), 8u);
        ASSERT_EQ(sizeof(isize), 8u);
        ASSERT_EQ(sizeof(usize), 8u);
    }
    else {
        ASSERT_EQ(sizeof(iptr), 4u);
        ASSERT_EQ(sizeof(uptr), 4u);
        ASSERT_EQ(sizeof(isize), 4u);
        ASSERT_EQ(sizeof(usize), 4u);
    }
}


TEST(Types, signed_unsigned) {
   ASSERT_TRUE(hud::is_unsigned_v<bool>);
   ASSERT_TRUE(hud::is_signed_v<i8>);
   ASSERT_TRUE(hud::is_signed_v<i16>);
   ASSERT_TRUE(hud::is_signed_v<i32>);
   ASSERT_TRUE(hud::is_signed_v<i64>);
   ASSERT_TRUE(hud::is_unsigned_v<u8>);
   ASSERT_TRUE(hud::is_unsigned_v<u16>);
   ASSERT_TRUE(hud::is_unsigned_v<u32>);
   ASSERT_TRUE(hud::is_unsigned_v<u64>);
   ASSERT_TRUE(hud::is_signed_v<ansichar>);
   #if defined(HD_OS_WINDOWS)
   ASSERT_TRUE(hud::is_unsigned_v<wchar>);
   #else // HD_OS_LINUX
   ASSERT_TRUE(hud::is_signed_v<wchar>);
   #endif 
   ASSERT_TRUE(hud::is_signed_v<f32>);
   ASSERT_TRUE(hud::is_signed_v<f64>);
   ASSERT_TRUE(hud::is_signed_v<iptr>);
   ASSERT_TRUE(hud::is_unsigned_v<uptr>);
   ASSERT_TRUE(hud::is_signed_v<isize>);
   ASSERT_TRUE(hud::is_unsigned_v<usize>);
}


TEST(Types, limits) {
    ASSERT_EQ(hud::u8_max, std::numeric_limits<u8>::max());
    ASSERT_EQ(hud::u8_min, std::numeric_limits<u8>::min());
    ASSERT_EQ(hud::u16_max, std::numeric_limits<u16>::max());
    ASSERT_EQ(hud::u16_min, std::numeric_limits<u16>::min());
    ASSERT_EQ(hud::u32_max, std::numeric_limits<u32>::max());
    ASSERT_EQ(hud::u32_min, std::numeric_limits<u32>::min());
    ASSERT_EQ(hud::u64_max, std::numeric_limits<u64>::max());
    ASSERT_EQ(hud::u64_min, std::numeric_limits<u64>::min());

    ASSERT_EQ(hud::i8_max, std::numeric_limits<i8>::max());
    ASSERT_EQ(hud::i8_min, std::numeric_limits<i8>::min());
    ASSERT_EQ(hud::i16_max, std::numeric_limits<i16>::max());
    ASSERT_EQ(hud::i16_min, std::numeric_limits<i16>::min());
    ASSERT_EQ(hud::i32_max, std::numeric_limits<i32>::max());
    ASSERT_EQ(hud::i32_min, std::numeric_limits<i32>::min());
    ASSERT_EQ(hud::i64_max, std::numeric_limits<i64>::max());
    ASSERT_EQ(hud::i64_min, std::numeric_limits<i64>::min());

    ASSERT_EQ(hud::ansichar_max, std::numeric_limits<ansichar>::max());
    ASSERT_EQ(hud::ansichar_min, std::numeric_limits<ansichar>::min());

    ASSERT_EQ(hud::wchar_max, std::numeric_limits<wchar>::max());
    ASSERT_EQ(hud::wchar_min, std::numeric_limits<wchar>::min());

    ASSERT_EQ(hud::f32_max, std::numeric_limits<f32>::max());
    ASSERT_EQ(hud::f32_min, std::numeric_limits<f32>::min());
    ASSERT_EQ(hud::f64_max, std::numeric_limits<f64>::max());
    ASSERT_EQ(hud::f64_min, std::numeric_limits<f64>::min());

    ASSERT_EQ(hud::uptr_max, std::numeric_limits<uptr>::max());
    ASSERT_EQ(hud::uptr_min, std::numeric_limits<uptr>::min());
    ASSERT_EQ(hud::iptr_max, std::numeric_limits<iptr>::max());
    ASSERT_EQ(hud::iptr_min, std::numeric_limits<iptr>::min());

    ASSERT_EQ(hud::usize_max, std::numeric_limits<usize>::max());
    ASSERT_EQ(hud::usize_min, std::numeric_limits<usize>::min());
    ASSERT_EQ(hud::isize_max, std::numeric_limits<isize>::max());
    ASSERT_EQ(hud::isize_min, std::numeric_limits<isize>::min());
}