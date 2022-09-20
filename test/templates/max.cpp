#include <core/templates/max.h>

TEST(templates, max) {


    ASSERT_TRUE(hud::max(1, 12.0f) == 12.0f);
    ASSERT_TRUE(hud::max(12.0f, 1) == 12.0f);
    ASSERT_TRUE(hud::max(12.0f, 12.0f) == 12.0f);
}
