#include <core/traits/add_const.h>

GTEST_TEST(traits, add_const)
{
    hud_assert_true((hud::is_same_v<hud::add_const_t<i32>, const i32>));
    hud_assert_true((hud::is_same_v<hud::add_const_t<const i32>, const i32>));
    hud_assert_true((hud::is_same_v<hud::add_const_t<volatile i32>, const volatile i32>));
    hud_assert_true((hud::is_same_v<hud::add_const_t<i32 *>, i32 *const>));
    hud_assert_true((hud::is_same_v<hud::add_const_t<i32 *volatile>, i32 *const volatile>));
}