#include <core/traits/is_union.h>

namespace hud_test {
    class a {};
    union b {};
    struct c {
        union d {};
    };
}

GTEST_TEST(traits, is_union) {
    ASSERT_FALSE((hud::is_union_v<i32>));
    ASSERT_FALSE((hud::is_union_v<i32&>));
    ASSERT_FALSE((hud::is_union_v<i32[]>));
    ASSERT_FALSE((hud::is_union_v<i32[32]>));
    ASSERT_FALSE((hud::is_union_v<i32*>));

    ASSERT_FALSE((hud::is_union_v<hud_test::a>));
    ASSERT_TRUE((hud::is_union_v<hud_test::b>));
    ASSERT_FALSE((hud::is_union_v<hud_test::c>));
    ASSERT_TRUE((hud::is_union_v<hud_test::c::d>));
}