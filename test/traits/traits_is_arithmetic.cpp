#include <core/traits/is_arithmetic.h>

namespace hud_test
{
    enum a
    {
    };

    class b
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_arithmetic)
{
    hud_assert_true(hud::is_arithmetic_v<bool>);
    hud_assert_true(hud::is_arithmetic_v<i8>);
    hud_assert_true(hud::is_arithmetic_v<i16>);
    hud_assert_true(hud::is_arithmetic_v<i32>);
    hud_assert_true(hud::is_arithmetic_v<i64>);
    hud_assert_true(hud::is_arithmetic_v<u8>);
    hud_assert_true(hud::is_arithmetic_v<u16>);
    hud_assert_true(hud::is_arithmetic_v<u32>);
    hud_assert_true(hud::is_arithmetic_v<u64>);
    hud_assert_true(hud::is_arithmetic_v<char8>);
    hud_assert_true(hud::is_arithmetic_v<wchar>);
    hud_assert_true(hud::is_arithmetic_v<char16>);
    hud_assert_true(hud::is_arithmetic_v<char32>);
    hud_assert_false(hud::is_arithmetic_v<hud_test::a>);
    hud_assert_false(hud::is_arithmetic_v<hud_test::b>);
}