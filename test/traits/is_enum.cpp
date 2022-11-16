#include <core/traits/is_enum.h>

namespace hud_test {
    enum a { first, second, third };       // unscoped enum (c-style)
    class b {};
    enum class c { alpha, beta, gamma };   // scoped enum (c++11-style)
}

GTEST_TEST(traits, is_enum) {
    ASSERT_FALSE((hud::is_enum_v<i32>));
    ASSERT_TRUE((hud::is_enum_v<hud_test::a>));
    ASSERT_FALSE((hud::is_enum_v<hud_test::b>));
    ASSERT_TRUE((hud::is_enum_v<hud_test::c>));
    ASSERT_TRUE((hud::is_enum_v<decltype(hud_test::c::alpha)>));
}