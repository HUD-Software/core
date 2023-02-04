#include <core/traits/is_base_of.h>

namespace hud_test
{
    class a
    {
    };

    class b : a
    {
    };

    class c
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_base_of)
{

    hud_assert_true((hud::is_base_of_v<hud_test::a, hud_test::a>));
    hud_assert_true((hud::is_base_of_v<hud_test::a, hud_test::b>));
    hud_assert_false((hud::is_base_of_v<hud_test::a, hud_test::c>));
    hud_assert_false((hud::is_base_of_v<hud_test::b, hud_test::a>));
    hud_assert_true((hud::is_base_of_v<hud_test::b, hud_test::b>));
    hud_assert_false((hud::is_base_of_v<hud_test::b, hud_test::c>));
    hud_assert_false((hud::is_base_of_v<hud_test::c, hud_test::a>));
    hud_assert_false((hud::is_base_of_v<hud_test::c, hud_test::b>));
    hud_assert_true((hud::is_base_of_v<hud_test::c, hud_test::c>));

    hud_assert_false((hud::is_base_of_v<hud_test::a *, hud_test::a *>));
    hud_assert_false((hud::is_base_of_v<hud_test::a *, hud_test::b *>));
    hud_assert_false((hud::is_base_of_v<hud_test::a *, hud_test::c *>));
}
