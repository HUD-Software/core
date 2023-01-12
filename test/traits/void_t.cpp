#include <core/traits/void_t.h>

GTEST_TEST(traits, void_t)
{
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::void_t<>, void>));
}