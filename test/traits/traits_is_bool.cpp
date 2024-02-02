#include <core/traits/is_bool.h>

GTEST_TEST(traits, is_bool)
{
    hud_assert_true(hud::is_bool_v<bool>);
    hud_assert_true(hud::is_bool_v<const bool>);
    hud_assert_true(hud::is_bool_v<volatile bool>);
    hud_assert_true(hud::is_bool_v<const volatile bool>);
}