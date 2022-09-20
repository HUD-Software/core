#include <core/traits/add_cv.h>

TEST(traits, add_cv) {
    ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32>, const volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<const i32>, const volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<volatile i32>, const volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32*>, i32* const volatile>));
    ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32* volatile>, i32* const volatile>));
    ASSERT_TRUE((hud::is_same_v<hud::add_cv_t<i32* const>, i32* const volatile>));
}