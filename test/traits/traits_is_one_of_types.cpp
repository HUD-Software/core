#include <core/traits/is_one_of_types.h>

GTEST_TEST(traits, is_one_of_types)
{
    hud_assert_true((hud::is_one_of_types_v<bool, int, long, float, short, bool, wchar_t>));
    hud_assert_true((hud::is_one_of_types_v<int, int, long, float, short, bool, wchar_t>));
    hud_assert_true((hud::is_one_of_types_v<wchar_t, int, long, float, short, bool, wchar_t>));
    hud_assert_false((hud::is_one_of_types_v<bool, int, long, float, short, wchar_t>));
}