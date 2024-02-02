#include <core/traits/is_empty.h>

namespace hud_test
{

    struct empty
    {
    };

    struct empty_2
    {
        static bool var;
    };

    struct empty_3
    {
        void f()
        {
        }
    };

    struct non_empty
    {
        int i;
    };

    struct inherited_empty : empty
    {
    };

    struct inherited_non_empty : non_empty
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_empty)
{
    hud_assert_true(hud::is_empty_v<hud_test::empty>);
    hud_assert_true(hud::is_empty_v<hud_test::empty_2>);
    hud_assert_true(hud::is_empty_v<hud_test::empty_3>);

    hud_assert_false(hud::is_empty_v<hud_test::non_empty>);
    hud_assert_true(hud::is_empty_v<hud_test::inherited_empty>);
    hud_assert_false(hud::is_empty_v<hud_test::inherited_non_empty>);
}
