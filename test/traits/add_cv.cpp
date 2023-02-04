#include <core/traits/add_cv.h>

GTEST_TEST(traits, add_cv)
{
    hud_assert_true((hud::is_same_v<hud::add_cv_t<i32>, const volatile i32>));
    hud_assert_true((hud::is_same_v<hud::add_cv_t<const i32>, const volatile i32>));
    hud_assert_true((hud::is_same_v<hud::add_cv_t<volatile i32>, const volatile i32>));
    hud_assert_true((hud::is_same_v<hud::add_cv_t<i32 *>, i32 *const volatile>));
    hud_assert_true((hud::is_same_v<hud::add_cv_t<i32 *volatile>, i32 *const volatile>));
    hud_assert_true((hud::is_same_v<hud::add_cv_t<i32 *const>, i32 *const volatile>));
}