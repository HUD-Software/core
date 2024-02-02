#include <core/traits/add_pointer.h>

GTEST_TEST(traits, add_pointer)
{
    hud_assert_true((hud::is_same_v<hud::add_pointer_t<i32>, i32 *>));
    hud_assert_true((hud::is_same_v<hud::add_pointer_t<i32 &>, i32 *>));
    hud_assert_true((hud::is_same_v<hud::add_pointer_t<i32 &&>, i32 *>));
    hud_assert_true((hud::is_same_v<hud::add_pointer_t<i32 *>, i32 **>));
}