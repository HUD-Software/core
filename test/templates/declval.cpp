#include <core/templates/declval.h>

TEST(templates, declval) {
    i32 integer = 22;
    decltype(hud::declval<i32>()) n = hud::move(integer);
    ASSERT_EQ(n, integer);
}