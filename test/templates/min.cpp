#include <core/templates/min.h>

GTEST_TEST(templates, min)
{
    hud_assert_true(hud::min(1, 12) == 1);
    hud_assert_true(hud::min(12, 1) == 1);
    hud_assert_true(hud::min(12, 12) == 12);
}
