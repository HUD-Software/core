#include <core/traits/void_t.h>

GTEST_TEST(traits, void_t)
{
    hud_assert_true((hud::is_same_v<hud::void_t<>, void>));
}