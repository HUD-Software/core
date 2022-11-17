#include <core/traits/is_trivial.h>

namespace hud_test {
    class a {};
    class b { b() {} };
    class c : b {};
    class d { virtual void fn() {} };
}

GTEST_TEST(traits, is_trivial) {
    GTEST_ASSERT_TRUE((hud::is_trivial_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_trivial_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_trivial_v<hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_trivial_v<hud_test::d>));
}