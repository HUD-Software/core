#include <core/traits/add_volatile.h>

GTEST_TEST(traits, add_volatile)
{
    hud_assert_true((hud::is_same_v<hud::add_volatile_t<i32>, volatile i32>));
    hud_assert_true((hud::is_same_v<hud::add_volatile_t<const i32>, const volatile i32>));
    hud_assert_true((hud::is_same_v<hud::add_volatile_t<volatile i32>, volatile i32>));
    hud_assert_true((hud::is_same_v<hud::add_volatile_t<i32 *>, i32 *volatile>));
    hud_assert_true((hud::is_same_v<hud::add_volatile_t<i32 *volatile>, i32 *volatile>));
}