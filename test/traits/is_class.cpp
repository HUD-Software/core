#include <core/traits/is_class.h>

TEST(traits, is_class) {
    ASSERT_FALSE(hud::is_class_v<i32>);
    ASSERT_FALSE(hud::is_class_v<i32*>);
    ASSERT_FALSE(hud::is_class_v<i32&>);
    ASSERT_TRUE(hud::is_class_v<class a>);
    ASSERT_TRUE(hud::is_class_v<struct S>);
}