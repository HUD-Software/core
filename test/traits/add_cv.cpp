#include <core/traits/add_cv.h>

GTEST_TEST(traits, add_cv) {
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32>, const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<const i32>, const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<volatile i32>, const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32*>, i32* const volatile>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32* volatile>, i32* const volatile>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32* const>, i32* const volatile>));
}