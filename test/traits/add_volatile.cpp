#include <core/traits/add_volatile.h>

GTEST_TEST(traits, add_volatile)
{
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_volatile_t<i32>, volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_volatile_t<const i32>, const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_volatile_t<volatile i32>, volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_volatile_t<i32 *>, i32 *volatile>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_volatile_t<i32 *volatile>, i32 *volatile>));
}