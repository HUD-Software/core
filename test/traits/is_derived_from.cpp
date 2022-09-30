#include <core/traits/is_derived_from.h>

namespace hud_test {
    struct a {};
    struct b : public a {};
}

TEST(traits, is_derived_from) {
    ASSERT_FALSE((hud::is_derived_from_v<hud_test::a, hud_test::a>));
    ASSERT_FALSE((hud::is_derived_from_v<hud_test::a, hud_test::b>));
    ASSERT_TRUE((hud::is_derived_from_v<hud_test::b, hud_test::a>));
    ASSERT_FALSE((hud::is_derived_from_v<hud_test::b, hud_test::b>));
}
