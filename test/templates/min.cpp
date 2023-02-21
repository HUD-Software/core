#include <core/templates/min.h>

GTEST_TEST(templates, min)
{
    hud_assert_true(hud::min(1, 12.0f) == 1);
    hud_assert_true(hud::min(12.0f, 1) == 1);
    hud_assert_true(hud::min(12.0f, 12.0f) == 12.0f);
}
