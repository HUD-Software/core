#include <core/traits/is_transparent.h>

namespace hud_test
{
    struct transparent
    {
        using is_transparent = void;
    };

    struct not_transparent
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_transparent)
{
    hud_assert_true(hud::is_transparent_v<hud_test::transparent>);
    hud_assert_false(hud::is_transparent_v<hud_test::not_transparent>);
}