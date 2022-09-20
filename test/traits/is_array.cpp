#include <core/traits/is_array.h>

namespace {
    class a {};
}

TEST(traits, is_array) {
    ASSERT_FALSE(hud::is_array_v<a>);
    ASSERT_FALSE(hud::is_array_v<a*>);
    ASSERT_TRUE(hud::is_array_v<a[]>);
    ASSERT_TRUE(hud::is_array_v<a[][2]>);
    ASSERT_TRUE(hud::is_array_v<a[2]>);
    ASSERT_TRUE(hud::is_array_v<a[2][2]>);
}