#include <core/templates/max.h>

GTEST_TEST(templates, max) {


    GTEST_ASSERT_TRUE(hud::max(1, 12.0f) == 12.0f);
    GTEST_ASSERT_TRUE(hud::max(12.0f, 1) == 12.0f);
    GTEST_ASSERT_TRUE(hud::max(12.0f, 12.0f) == 12.0f);
}
