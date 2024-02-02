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
    hud_assert_false((hud::is_enum_v<i32>));
    hud_assert_true((hud::is_enum_v<hud_test::a>));
    hud_assert_false((hud::is_enum_v<hud_test::b>));
    hud_assert_true((hud::is_enum_v<hud_test::c>));
    hud_assert_true((hud::is_enum_v<decltype(hud_test::c::alpha)>));
}