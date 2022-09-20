#include <core/traits/is_union.h>

namespace {
    class a {};
    union b {};
    struct c {
        union d {};
    };
}

TEST(traits, is_union) {
    ASSERT_FALSE((hud::is_union_v<i32>));
    ASSERT_FALSE((hud::is_union_v<i32&>));
    ASSERT_FALSE((hud::is_union_v<i32[]>));
    ASSERT_FALSE((hud::is_union_v<i32[32]>));
    ASSERT_FALSE((hud::is_union_v<i32*>));

    ASSERT_FALSE((hud::is_union_v<a>));
    ASSERT_TRUE((hud::is_union_v<b>));
    ASSERT_FALSE((hud::is_union_v<c>));
    ASSERT_TRUE((hud::is_union_v<c::d>));
}