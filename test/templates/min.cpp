#include <core/templates/min.h>

GTEST_TEST(templates, min) {


    GTEST_ASSERT_TRUE(hud::min(1, 12.0f) == 1);
    GTEST_ASSERT_TRUE(hud::min(12.0f, 1) == 1);
    GTEST_ASSERT_TRUE(hud::min(12.0f, 12.0f) == 12.0f);
}
