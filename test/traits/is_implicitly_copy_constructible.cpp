#include <core/traits/is_implicitly_copy_constructible.h>

namespace hud_test
{
    struct no_explicit_ctor
    {
        no_explicit_ctor(const no_explicit_ctor &) noexcept {};
    };

    struct explicit_ctor
    {
        explicit explicit_ctor(const explicit_ctor &) noexcept {};
    };
} // namespace hud_test

GTEST_TEST(traits, is_implicitly_copy_constructible)
{
    hud_assert_false((hud::is_implicitly_copy_constructible_v<hud_test::explicit_ctor>));
    hud_assert_true((hud::is_implicitly_copy_constructible_v<hud_test::no_explicit_ctor>));
    hud_assert_true((hud::is_implicitly_copy_constructible_v<i32>));
    hud_assert_true((hud::is_implicitly_copy_constructible_v<const i32>));
    hud_assert_true((hud::is_implicitly_copy_constructible_v<volatile i32>));
    hud_assert_true((hud::is_implicitly_copy_constructible_v<const volatile i32>));
}