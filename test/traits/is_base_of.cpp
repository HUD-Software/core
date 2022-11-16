#include <core/traits/is_base_of.h>

namespace hud_test {
    class a {};
    class b : a {};
    class c {};
}

GTEST_TEST(traits, is_base_of) {

    ASSERT_TRUE((hud::is_base_of_v<hud_test::a, hud_test::a>));
    ASSERT_TRUE((hud::is_base_of_v<hud_test::a, hud_test::b>));
    ASSERT_FALSE((hud::is_base_of_v<hud_test::a, hud_test::c>));
    ASSERT_FALSE((hud::is_base_of_v<hud_test::b, hud_test::a>));
    ASSERT_TRUE((hud::is_base_of_v<hud_test::b, hud_test::b>));
    ASSERT_FALSE((hud::is_base_of_v<hud_test::b, hud_test::c>));
    ASSERT_FALSE((hud::is_base_of_v<hud_test::c, hud_test::a>));
    ASSERT_FALSE((hud::is_base_of_v<hud_test::c, hud_test::b>));
    ASSERT_TRUE((hud::is_base_of_v<hud_test::c, hud_test::c>));

    ASSERT_FALSE((hud::is_base_of_v<hud_test::a*, hud_test::a*>));
    ASSERT_FALSE((hud::is_base_of_v<hud_test::a*, hud_test::b*>));
    ASSERT_FALSE((hud::is_base_of_v<hud_test::a*, hud_test::c*>));
}
