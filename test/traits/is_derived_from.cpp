#include <core/traits/is_derived_from.h>

namespace hud_test
{
    struct a
    {
    };

    struct b : public a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_derived_from)
{
    hud_assert_false((hud::is_derived_from_v<hud_test::a, hud_test::a>));
    hud_assert_false((hud::is_derived_from_v<hud_test::a, hud_test::b>));
    hud_assert_true((hud::is_derived_from_v<hud_test::b, hud_test::a>));
    hud_assert_false((hud::is_derived_from_v<hud_test::b, hud_test::b>));
}
