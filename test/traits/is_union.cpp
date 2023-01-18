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
    GTEST_ASSERT_FALSE((hud::is_union_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_union_v<i32 &>));
    GTEST_ASSERT_FALSE((hud::is_union_v<i32[]>));
    GTEST_ASSERT_FALSE((hud::is_union_v<i32[32]>));
    GTEST_ASSERT_FALSE((hud::is_union_v<i32 *>));

    GTEST_ASSERT_FALSE((hud::is_union_v<hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_union_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_union_v<hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_union_v<hud_test::c::d>));
}