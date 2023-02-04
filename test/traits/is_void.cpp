#include <core/traits/is_void.h>

namespace hud_test
{
    class a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_void)
{
    hud_assert_false((hud::is_void_v<i32>));
    hud_assert_false((hud::is_void_v<i32 &>));
    hud_assert_false((hud::is_void_v<i32[]>));
    hud_assert_false((hud::is_void_v<i32[32]>));
    hud_assert_false((hud::is_void_v<i32 *>));

    hud_assert_true((hud::is_void_v<void>));
    hud_assert_true((hud::is_void_v<const void>));
    hud_assert_true((hud::is_void_v<volatile void>));
    hud_assert_true((hud::is_void_v<const volatile void>));
    hud_assert_true((hud::is_void_v<hud::void_t<>>));
    hud_assert_true((hud::is_void_v<const hud::void_t<>>));
    hud_assert_true((hud::is_void_v<volatile hud::void_t<>>));
    hud_assert_true((hud::is_void_v<const volatile hud::void_t<>>));
    hud_assert_true((hud::is_void_v<decltype(void())>));
    hud_assert_false((hud::is_void_v<hud_test::a>));
}
