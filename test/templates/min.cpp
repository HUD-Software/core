#include <core/templates/min.h>

TEST(templates, min) {


    ASSERT_TRUE(hud::min(1, 12.0f) == 1);
    ASSERT_TRUE(hud::min(12.0f, 1) == 1);
    ASSERT_TRUE(hud::min(12.0f, 12.0f) == 12.0f);
}
