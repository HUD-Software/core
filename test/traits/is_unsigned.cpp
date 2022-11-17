#include <core/traits/is_unsigned.h>

namespace hud_test {
    struct a {};
}

GTEST_TEST(traits, is_unsigned) {
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<bool>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<i8>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<i16>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<i64>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<iptr>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<f32>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<f64>));

    GTEST_ASSERT_TRUE((hud::is_unsigned_v<u8>));
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<u16>));
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<u32>));
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<u64>));
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<uptr>));

    GTEST_ASSERT_FALSE((hud::is_unsigned_v<isize>));
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<usize>));
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<char16>));
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<char32>));
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<ansichar>));
    #if defined(HD_OS_WINDOWS)
    GTEST_ASSERT_TRUE((hud::is_unsigned_v<wchar>));
    #else // HD_OS_LINUX
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<wchar>));
    #endif
    GTEST_ASSERT_FALSE((hud::is_unsigned_v<hud_test::a>));
}
