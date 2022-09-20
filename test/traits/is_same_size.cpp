#include <core/traits/is_same_size.h>

namespace {
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
    ASSERT_TRUE((hud::is_same_size_v<a, std::tuple<>>));
    ASSERT_FALSE((hud::is_same_size_v<a, b>));
    ASSERT_FALSE((hud::is_same_size_v<b, c>));
    ASSERT_TRUE((hud::is_same_size_v<b, d>));
    ASSERT_TRUE((hud::is_same_size_v<b, i32>));
    ASSERT_TRUE((hud::is_same_size_v<b[2], i32[1]>));
}