#include <core/traits/is_base_of.h>

namespace {
    class a {};
    class b : a {};
    class c {};
}

TEST(traits, is_base_of) {

    ASSERT_TRUE((hud::is_base_of_v<a, a>));
    ASSERT_TRUE((hud::is_base_of_v<a, b>));
    ASSERT_FALSE((hud::is_base_of_v<a, c>));
    ASSERT_FALSE((hud::is_base_of_v<b, a>));
    ASSERT_TRUE((hud::is_base_of_v<b, b>));
    ASSERT_FALSE((hud::is_base_of_v<b, c>));
    ASSERT_FALSE((hud::is_base_of_v<c, a>));
    ASSERT_FALSE((hud::is_base_of_v<c, b>));
    ASSERT_TRUE((hud::is_base_of_v<c, c>));

    ASSERT_FALSE((hud::is_base_of_v<a*, a*>));
    ASSERT_FALSE((hud::is_base_of_v<a*, b*>));
    ASSERT_FALSE((hud::is_base_of_v<a*, c*>));
}
