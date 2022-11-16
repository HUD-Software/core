#include <core/traits/is_unsigned.h>

namespace hud_test {
    struct a {};
}

GTEST_TEST(traits, is_unsigned) {
    ASSERT_TRUE((hud::is_unsigned_v<bool>));
    ASSERT_FALSE((hud::is_unsigned_v<i8>));
    ASSERT_FALSE((hud::is_unsigned_v<i16>));
    ASSERT_FALSE((hud::is_unsigned_v<i32>));
    ASSERT_FALSE((hud::is_unsigned_v<i64>));
    ASSERT_FALSE((hud::is_unsigned_v<iptr>));
    ASSERT_FALSE((hud::is_unsigned_v<f32>));
    ASSERT_FALSE((hud::is_unsigned_v<f64>));

    ASSERT_TRUE((hud::is_unsigned_v<u8>));
    ASSERT_TRUE((hud::is_unsigned_v<u16>));
    ASSERT_TRUE((hud::is_unsigned_v<u32>));
    ASSERT_TRUE((hud::is_unsigned_v<u64>));
    ASSERT_TRUE((hud::is_unsigned_v<uptr>));

    ASSERT_FALSE((hud::is_unsigned_v<isize>));
    ASSERT_TRUE((hud::is_unsigned_v<usize>));
    ASSERT_TRUE((hud::is_unsigned_v<char16>));
    ASSERT_TRUE((hud::is_unsigned_v<char32>));
    ASSERT_FALSE((hud::is_unsigned_v<ansichar>));
    #if defined(HD_OS_WINDOWS)
    ASSERT_TRUE((hud::is_unsigned_v<wchar>));
    #else // HD_OS_LINUX
    ASSERT_FALSE((hud::is_unsigned_v<wchar>));
    #endif
    ASSERT_FALSE((hud::is_unsigned_v<hud_test::a>));
}
