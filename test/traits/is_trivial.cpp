#include <core/traits/is_trivial.h>

namespace {
    class a {};
    class b { b() {} };
    class c : b {};
    class d { virtual void fn() {} };
}

TEST(traits, is_trivial) {
    ASSERT_TRUE((hud::is_trivial_v<a>));
    ASSERT_FALSE((hud::is_trivial_v<b>));
    ASSERT_FALSE((hud::is_trivial_v<c>));
    ASSERT_FALSE((hud::is_trivial_v<d>));
}