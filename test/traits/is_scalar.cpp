#include <core/traits/is_scalar.h>

namespace hud_test {
    struct a { void fn() {} };
    enum class e {};
}

GTEST_TEST(traits, is_scalar) {
    GTEST_ASSERT_FALSE((hud::is_scalar_v<hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<hud_test::e>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<i8>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<i16>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<i64>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<iptr>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<u8>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<u16>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<u32>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<u64>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<uptr>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<isize>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<usize>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<ansichar>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<wchar>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<hud_test::a*>));
    GTEST_ASSERT_TRUE((hud::is_scalar_v<decltype(&hud_test::a::fn)>));
    GTEST_ASSERT_FALSE((hud::is_scalar_v<int(int)>));
}