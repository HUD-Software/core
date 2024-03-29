#include <core/traits/is_union.h>

namespace hud_test
{
    class a
    {
    };

    union b
    {
    };

    struct c
    {
        union d
        {
        };
    };
} // namespace hud_test

GTEST_TEST(traits, is_union)
{
    hud_assert_false((hud::is_union_v<i32>));
    hud_assert_false((hud::is_union_v<i32 &>));
    hud_assert_false((hud::is_union_v<i32[]>));
    hud_assert_false((hud::is_union_v<i32[32]>));
    hud_assert_false((hud::is_union_v<i32 *>));

    hud_assert_false((hud::is_union_v<hud_test::a>));
    hud_assert_true((hud::is_union_v<hud_test::b>));
    hud_assert_false((hud::is_union_v<hud_test::c>));
    hud_assert_true((hud::is_union_v<hud_test::c::d>));
}