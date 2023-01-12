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
}

GTEST_TEST(traits, is_implicitly_copy_constructible)
{
    GTEST_ASSERT_FALSE((hud::is_implicitly_copy_constructible_v<hud_test::explicit_ctor>));
    GTEST_ASSERT_TRUE((hud::is_implicitly_copy_constructible_v<hud_test::no_explicit_ctor>));
    GTEST_ASSERT_TRUE((hud::is_implicitly_copy_constructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_implicitly_copy_constructible_v<const i32>));
    GTEST_ASSERT_TRUE((hud::is_implicitly_copy_constructible_v<volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_implicitly_copy_constructible_v<const volatile i32>));
}