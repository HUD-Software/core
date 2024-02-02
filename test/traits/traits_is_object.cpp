#include <core/traits/is_object.h>

namespace hud_test
{
    class a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_object)
{
    hud_assert_true((hud::is_object_v<i32>));
    hud_assert_true((hud::is_object_v<hud_test::a>));
    hud_assert_false((hud::is_object_v<hud_test::a &>));
    hud_assert_true((hud::is_object_v<hud_test::a *>));
    hud_assert_false((hud::is_object_v<i32(i32)>));
    hud_assert_true((hud::is_object_v<i32 (*)(i32)>));
}