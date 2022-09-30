#include <core/traits/is_arithmetic.h>

namespace hud_test {
    enum a {};
    class b {};
}

TEST(traits, is_arithmetic) {
    ASSERT_TRUE(hud::is_arithmetic_v<bool>);
    ASSERT_TRUE(hud::is_arithmetic_v<i8>);
    ASSERT_TRUE(hud::is_arithmetic_v<i16>);
    ASSERT_TRUE(hud::is_arithmetic_v<i32>);
    ASSERT_TRUE(hud::is_arithmetic_v<i64>);
    ASSERT_TRUE(hud::is_arithmetic_v<u8>);
    ASSERT_TRUE(hud::is_arithmetic_v<u16>);
    ASSERT_TRUE(hud::is_arithmetic_v<u32>);
    ASSERT_TRUE(hud::is_arithmetic_v<u64>);
    ASSERT_TRUE(hud::is_arithmetic_v<ansichar>);
    ASSERT_TRUE(hud::is_arithmetic_v<wchar>);
    ASSERT_TRUE(hud::is_arithmetic_v<char16>);
    ASSERT_TRUE(hud::is_arithmetic_v<char32>);
    ASSERT_FALSE(hud::is_arithmetic_v<hud_test::a>);
    ASSERT_FALSE(hud::is_arithmetic_v<hud_test::b>);
}