#include <core/templates/max.h>

GTEST_TEST(templates, max)
{

    hud_assert_true(hud::max(1, 12.0f) == 12.0f);
    hud_assert_true(hud::max(12.0f, 1) == 12.0f);
    hud_assert_true(hud::max(12.0f, 12.0f) == 12.0f);
}
