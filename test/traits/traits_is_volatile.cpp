#include <core/traits/is_volatile.h>

GTEST_TEST(traits, is_volatile)
{
    hud_assert_false((hud::is_volatile_v<int>));
    hud_assert_true((hud::is_volatile_v<volatile int>));
    hud_assert_false((hud::is_volatile_v<const int>));
    hud_assert_true((hud::is_volatile_v<const volatile int>));
    hud_assert_false((hud::is_volatile_v<volatile int *>));
    hud_assert_true((hud::is_volatile_v<volatile int *volatile>));
}
