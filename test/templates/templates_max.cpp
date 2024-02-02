#include <core/templates/max.h>

GTEST_TEST(templates, max)
{
    hud_assert_true(hud::max(1, 12) == 12);
    hud_assert_true(hud::max(12, 1) == 12);
    hud_assert_true(hud::max(12, 12) == 12);
}
