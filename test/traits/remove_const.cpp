#include <core/traits/remove_const.h>

GTEST_TEST(traits, remove_const)
{
    hud_assert_true((hud::is_same_v<hud::remove_const_t<i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_const_t<const i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_const_t<volatile i32>, volatile i32>));
    hud_assert_true((hud::is_same_v<hud::remove_const_t<const volatile i32>, volatile i32>));
    hud_assert_true((hud::is_same_v<hud::remove_const_t<const i32 *const>, const i32 *>));
    hud_assert_true((hud::is_same_v<hud::remove_const_t<const volatile i32 *const volatile>, const volatile i32 *volatile>));
}