#include <core/traits/add_const.h>

GTEST_TEST(traits, add_const)
{
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_const_t<i32>, const i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_const_t<const i32>, const i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_const_t<volatile i32>, const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_const_t<i32 *>, i32 *const>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_const_t<i32 *volatile>, i32 *const volatile>));
}