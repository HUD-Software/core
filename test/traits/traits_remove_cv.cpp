#include <core/traits/remove_cv.h>

GTEST_TEST(traits, remove_cv)
{
    hud_assert_true((hud::is_same_v<hud::remove_cv_t<i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_cv_t<volatile i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_cv_t<const i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_cv_t<const volatile i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_cv_t<const i32 *const>, const i32 *>));
    hud_assert_true((hud::is_same_v<hud::remove_cv_t<const volatile i32 *const volatile>, const volatile i32 *>));
}
