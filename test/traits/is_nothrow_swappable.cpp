#include <core/traits/is_nothrow_swappable.h>

namespace hud_test
{
    struct moveable_2;

    struct movable
    {
        movable(movable &&);
        movable(moveable_2 &&);
    };

    struct moveable_2
    {
        moveable_2(movable &&);
        moveable_2(moveable_2 &&);
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

    struct nothrow_moveable_2;

    struct nothrow_moveable
    {
        nothrow_moveable(nothrow_moveable_2 &&) noexcept;
    };

    struct nothrow_moveable_2
    {
        nothrow_moveable_2(nothrow_moveable &&) noexcept;
    };

    struct nothrow_not_moveable_2;

    struct nothrow_not_moveable
    {
        nothrow_not_moveable(nothrow_not_moveable &&) noexcept = delete;
        nothrow_not_moveable(nothrow_not_moveable_2 &&) noexcept = delete;
        nothrow_not_moveable &operator=(nothrow_not_moveable &&) noexcept = delete;
    };

    struct nothrow_not_moveable_2
    {
        nothrow_not_moveable_2(nothrow_not_moveable_2 &&) noexcept = delete;
        nothrow_not_moveable_2 &operator=(nothrow_not_moveable_2 &&) noexcept = delete;
    };
} // namespace hud_test

GTEST_TEST(traits, is_nothrow_swappable)
{
    hud_assert_false(hud::is_nothrow_swappable_v<hud_test::movable>);
    hud_assert_false(hud::is_nothrow_swappable_v<hud_test::not_moveable>);

    hud_assert_false((hud::is_nothrow_swappable_v<hud_test::movable, hud_test::moveable_2>));
    hud_assert_false((hud::is_nothrow_swappable_v<hud_test::not_moveable, hud_test::not_moveable_2>));

    hud_assert_true(hud::is_nothrow_swappable_v<hud_test::nothrow_moveable>);
    hud_assert_false(hud::is_nothrow_swappable_v<hud_test::nothrow_not_moveable>);

    hud_assert_true((hud::is_nothrow_swappable_v<hud_test::nothrow_moveable, hud_test::nothrow_moveable_2>));
    hud_assert_false((hud::is_nothrow_swappable_v<hud_test::nothrow_not_moveable, hud_test::nothrow_not_moveable_2>));
}