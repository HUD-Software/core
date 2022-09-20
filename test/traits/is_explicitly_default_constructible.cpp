#include <core/traits/is_explicitly_default_constructible.h>

namespace {
    struct no_explicit_ctor {
        no_explicit_ctor() noexcept {};
    };
    struct explicit_ctor {
        explicit explicit_ctor() noexcept {}
    };
}

TEST(traits, is_explicitly_default_constructible) {
    ASSERT_TRUE((hud::is_explicitly_default_constructible_v<explicit_ctor>));
    ASSERT_FALSE((hud::is_explicitly_default_constructible_v<no_explicit_ctor>));
    ASSERT_FALSE((hud::is_explicitly_default_constructible_v<i32>));
    ASSERT_FALSE((hud::is_explicitly_default_constructible_v<const i32>));
    ASSERT_TRUE((hud::is_explicitly_default_constructible_v<volatile i32>));
    ASSERT_TRUE((hud::is_explicitly_default_constructible_v<const volatile i32>));
}
