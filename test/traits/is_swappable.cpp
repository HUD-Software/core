#include <core/traits/is_swappable.h>

namespace hud_test
{
    struct movaeble_2;
    struct movaeble
    {
        movaeble(movaeble_2 &&);
    };
    struct movaeble_2
    {
        movaeble_2(movaeble &&);
    };

    struct not_moveable_2;
    struct not_moveable
    {
        not_moveable(not_moveable &&) = delete;
        not_moveable(not_moveable_2 &&) = delete;
        not_moveable &operator=(not_moveable &&) = delete;
    };
    struct not_moveable_2
    {
        not_moveable_2(not_moveable_2 &&) = delete;
        not_moveable_2 &operator=(not_moveable_2 &&) = delete;
    };
}

GTEST_TEST(traits, is_swappable)
{
    GTEST_ASSERT_TRUE(hud::is_swappable_v<hud_test::movaeble>);
    GTEST_ASSERT_FALSE(hud::is_swappable_v<hud_test::not_moveable>);

    GTEST_ASSERT_TRUE((hud::is_swappable_v<hud_test::movaeble, hud_test::movaeble_2>));
    GTEST_ASSERT_FALSE((hud::is_swappable_v<hud_test::not_moveable, hud_test::not_moveable_2>));
}