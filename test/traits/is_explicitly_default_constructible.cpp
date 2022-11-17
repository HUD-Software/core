#include <core/traits/is_explicitly_default_constructible.h>

namespace hud_test {
    struct no_explicit_ctor {
        no_explicit_ctor() noexcept {};
    };
    struct explicit_ctor {
        explicit explicit_ctor() noexcept {}
    };
}

GTEST_TEST(traits, is_explicitly_default_constructible) {
    GTEST_ASSERT_TRUE((hud::is_explicitly_default_constructible_v<hud_test::explicit_ctor>));
    GTEST_ASSERT_FALSE((hud::is_explicitly_default_constructible_v<hud_test::no_explicit_ctor>));
    GTEST_ASSERT_FALSE((hud::is_explicitly_default_constructible_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_explicitly_default_constructible_v<const i32>));
    GTEST_ASSERT_TRUE((hud::is_explicitly_default_constructible_v<volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_explicitly_default_constructible_v<const volatile i32>));
}
