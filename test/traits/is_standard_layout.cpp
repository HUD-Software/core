#include <core/traits/is_standard_layout.h>

namespace hud_test
{
    struct a
    {
        int m;
    };

    struct b
    {
        int m1;

    private:
        [[maybe_unused]] int m2;
    };

    struct c
    {
        virtual void foo() {};
    };

    struct d : a
    {
        int j;
    };

    struct E
    {
        int m;
        short a;
        int b;
    };
} // namespace hud_test

GTEST_TEST(traits, is_standard_layout)
{
    hud_assert_true((hud::is_standard_layout_v<hud_test::a>));
    hud_assert_true((hud::is_standard_layout_v<i32>));
    hud_assert_true((hud::is_standard_layout_v<i32[]>));
    hud_assert_true((hud::is_standard_layout_v<i32[3]>));
    hud_assert_true((hud::is_standard_layout_v<i32 *>));
    hud_assert_false((hud::is_standard_layout_v<hud_test::b>));
    hud_assert_false((hud::is_standard_layout_v<hud_test::c>));
    hud_assert_false((hud::is_standard_layout_v<hud_test::d>));
    hud_assert_true((hud::is_standard_layout_v<hud_test::E>));
}
