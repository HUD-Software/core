#include <core/traits/remove_volatile.h>

GTEST_TEST(traits, remove_volatile)
{
    hud_assert_true((hud::is_same_v<hud::remove_volatile_t<i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_volatile_t<const i32>, const i32>));
    hud_assert_true((hud::is_same_v<hud::remove_volatile_t<volatile i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_volatile_t<const volatile i32>, const i32>));
    hud_assert_true((hud::is_same_v<hud::remove_volatile_t<const i32 *const>, const i32 *const>));
    hud_assert_true((hud::is_same_v<hud::remove_volatile_t<const volatile i32 *const volatile>, const volatile i32 *const>));
}