#include <core/traits/is_trivial.h>

namespace hud_test
{
    class a
    {
    };

    class b
    {
        b()
        {
        }
    };

    class c : b
    {
    };

    class d
    {
        virtual void fn()
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivial)
{
    hud_assert_true((hud::is_trivial_v<hud_test::a>));
    hud_assert_false((hud::is_trivial_v<hud_test::b>));
    hud_assert_false((hud::is_trivial_v<hud_test::c>));
    hud_assert_false((hud::is_trivial_v<hud_test::d>));
}