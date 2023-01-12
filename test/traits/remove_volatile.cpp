#include <core/traits/remove_volatile.h>

GTEST_TEST(traits, remove_volatile)
{
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_volatile_t<i32>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_volatile_t<const i32>, const i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_volatile_t<volatile i32>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_volatile_t<const volatile i32>, const i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_volatile_t<const i32 *const>, const i32 *const>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_volatile_t<const volatile i32 *const volatile>, const volatile i32 *const>));
}