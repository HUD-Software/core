#include <core/traits/is_trivial.h>

namespace hud_test {
    class a {};
    class b { b() {} };
    class c : b {};
    class d { virtual void fn() {} };
}

GTEST_TEST(traits, is_trivial) {
    ASSERT_TRUE((hud::is_trivial_v<hud_test::a>));
    ASSERT_FALSE((hud::is_trivial_v<hud_test::b>));
    ASSERT_FALSE((hud::is_trivial_v<hud_test::c>));
    ASSERT_FALSE((hud::is_trivial_v<hud_test::d>));
}