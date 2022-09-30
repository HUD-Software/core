#include <core/traits/is_same_size.h>

namespace hud_test {
    struct a {};
    struct b {
        i32 a;
    };
    struct c {
        i64 a;
    };
    struct d {
        f32 a;
    };
}

TEST(traits, is_same_size) {
    ASSERT_TRUE((hud::is_same_size_v<i32, const i32>));
    ASSERT_TRUE((hud::is_same_size_v<hud_test::a, std::tuple<>>));
    ASSERT_FALSE((hud::is_same_size_v<hud_test::a, hud_test::b>));
    ASSERT_FALSE((hud::is_same_size_v<hud_test::b, hud_test::c>));
    ASSERT_TRUE((hud::is_same_size_v<hud_test::b, hud_test::d>));
    ASSERT_TRUE((hud::is_same_size_v<hud_test::b, i32>));
    ASSERT_TRUE((hud::is_same_size_v<hud_test::b[2], i32[1]>));
}