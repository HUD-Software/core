#include <core/traits/add_pointer.h>

GTEST_TEST(traits, add_pointer) {
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_pointer_t<i32>, i32*>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_pointer_t<i32&>, i32*>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_pointer_t<i32&&>, i32*>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_pointer_t<i32*>, i32**>));
}