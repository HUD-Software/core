#include <core/traits/is_derived_from.h>

namespace {
    struct a {};
    struct b : public a {};
}

TEST(traits, is_derived_from) {
    ASSERT_FALSE((hud::is_derived_from_v<a, a>));
    ASSERT_FALSE((hud::is_derived_from_v<a, b>));
    ASSERT_TRUE((hud::is_derived_from_v<b, a>));
    ASSERT_FALSE((hud::is_derived_from_v<b, b>));
}
