#include <core/traits/is_explicitly_move_constructible.h>

namespace hud_test {
    struct no_explicit_ctor {
        no_explicit_ctor(no_explicit_ctor&&) noexcept {};
    };
    struct explicit_ctor {
        explicit explicit_ctor(explicit_ctor&&) noexcept {};
    };
}

GTEST_TEST(traits, is_explicitly_move_constructible) {
    ASSERT_TRUE((hud::is_explicitly_move_constructible_v<hud_test::explicit_ctor>));
    ASSERT_FALSE((hud::is_explicitly_move_constructible_v<hud_test::no_explicit_ctor>));
    ASSERT_FALSE((hud::is_explicitly_move_constructible_v<i32>));
    ASSERT_FALSE((hud::is_explicitly_move_constructible_v<const i32>));
    ASSERT_FALSE((hud::is_explicitly_move_constructible_v<volatile i32>));
    ASSERT_FALSE((hud::is_explicitly_move_constructible_v<const volatile i32>));
}
