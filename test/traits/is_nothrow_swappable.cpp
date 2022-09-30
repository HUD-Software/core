#include <core/traits/is_nothrow_swappable.h>

namespace hud_test {
    struct moveable_2;
    struct moveable {
        moveable(moveable&&);
        moveable(moveable_2&&);
    };
    struct moveable_2 {
        moveable_2(moveable&&);
        moveable_2(moveable_2&&);
    };

    struct not_moveable_2;
    struct not_moveable {
        not_moveable(not_moveable&&) = delete;
        not_moveable(not_moveable_2&&) = delete;
        not_moveable& operator=(not_moveable&&) = delete;
    };
    struct not_moveable_2 {
        not_moveable_2(not_moveable_2&&) = delete;
        not_moveable_2& operator=(not_moveable_2&&) = delete;
    };


    struct nothrow_moveable_2;
    struct nothrow_moveable {
        nothrow_moveable(nothrow_moveable_2&&) noexcept;
    };
    struct nothrow_moveable_2 {
        nothrow_moveable_2(nothrow_moveable&&) noexcept;
    };

    struct nothrow_not_moveable_2;
    struct nothrow_not_moveable {
        nothrow_not_moveable(nothrow_not_moveable&&) noexcept = delete;
        nothrow_not_moveable(nothrow_not_moveable_2&&) noexcept = delete;
        nothrow_not_moveable& operator=(nothrow_not_moveable&&) noexcept = delete;
    };
    struct nothrow_not_moveable_2 {
        nothrow_not_moveable_2(nothrow_not_moveable_2&&) noexcept = delete;
        nothrow_not_moveable_2& operator=(nothrow_not_moveable_2&&) noexcept = delete;
    };
}

TEST(traits, is_nothrow_swappable) {
    ASSERT_FALSE(hud::is_nothrow_swappable_v<hud_test::moveable>);
    ASSERT_FALSE(hud::is_nothrow_swappable_v<hud_test::not_moveable>);

    ASSERT_FALSE((hud::is_nothrow_swappable_v<hud_test::moveable, hud_test::moveable_2>));
    ASSERT_FALSE((hud::is_nothrow_swappable_v<hud_test::not_moveable, hud_test::not_moveable_2>));


    ASSERT_TRUE(hud::is_nothrow_swappable_v<hud_test::nothrow_moveable>);
    ASSERT_FALSE(hud::is_nothrow_swappable_v<hud_test::nothrow_not_moveable>);

    ASSERT_TRUE((hud::is_nothrow_swappable_v<hud_test::nothrow_moveable, hud_test::nothrow_moveable_2>));
    ASSERT_FALSE((hud::is_nothrow_swappable_v<hud_test::nothrow_not_moveable, hud_test::nothrow_not_moveable_2>));
}