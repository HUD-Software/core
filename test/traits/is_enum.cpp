#include <core/traits/is_enum.h>

namespace {
    enum a { first, second, third };       // unscoped enum (c-style)
    class b {};
    enum class c { alpha, beta, gamma };   // scoped enum (c++11-style)
}

TEST(traits, is_enum) {
    ASSERT_FALSE((hud::is_enum_v<i32>));
    ASSERT_TRUE((hud::is_enum_v<a>));
    ASSERT_FALSE((hud::is_enum_v<b>));
    ASSERT_TRUE((hud::is_enum_v<c>));
    ASSERT_TRUE((hud::is_enum_v<decltype(c::alpha)>));
}