#include <core/traits/is_enum.h>

namespace hud_test
{
    enum a
    {
        first,
        second,
        third
    }; // unscoped enum (c-style)

    class b
    {
    };
    enum class c
    {
        alpha,
        beta,
        gamma
    }; // scoped enum (c++11-style)
} // namespace hud_test

GTEST_TEST(traits, is_enum)
{
    GTEST_ASSERT_FALSE((hud::is_enum_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_enum_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_enum_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_enum_v<hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_enum_v<decltype(hud_test::c::alpha)>));
}