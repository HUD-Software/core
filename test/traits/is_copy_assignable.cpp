#include <core/traits/is_copy_assignable.h>

namespace hud_test
{
    struct b
    {
    };
    struct a
    {
        a &operator=(const b &) noexcept { return *this; }
    };
    struct c : public a
    {
    };
    struct d
    {
        d &operator=(const b &) noexcept { return *this; }
    };
}

GTEST_TEST(traits, is_copy_assignable)
{

    GTEST_ASSERT_TRUE((hud::is_copy_assignable_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_copy_assignable_v<const i32>));
    GTEST_ASSERT_FALSE((hud::is_copy_assignable_v<const volatile i32>));

    GTEST_ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_copy_assignable_v<const hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_copy_assignable_v<const volatile hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::c>)); // c = c compile
    GTEST_ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::d>)); // d = d do not compile no operator =
    GTEST_ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::a, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_copy_assignable_v<hud_test::a, hud_test::d>));
}