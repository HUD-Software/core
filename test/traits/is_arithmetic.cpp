#include <core/traits/is_arithmetic.h>

namespace hud_test {
    enum a {};
    class b {};
}

GTEST_TEST(traits, is_arithmetic) {
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<bool>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<i8>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<i16>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<i32>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<i64>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<u8>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<u16>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<u32>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<u64>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<ansichar>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<wchar>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<char16>);
    GTEST_ASSERT_TRUE(hud::is_arithmetic_v<char32>);
    GTEST_ASSERT_FALSE(hud::is_arithmetic_v<hud_test::a>);
    GTEST_ASSERT_FALSE(hud::is_arithmetic_v<hud_test::b>);
}