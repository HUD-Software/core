#include <core/traits/is_not_same.h>

namespace hud_test {
    typedef int integer_type;
    struct a { int x, y; };
    struct b { int x, y; };
    struct c : public a {};
    typedef a d;

    template<typename type_t>
    struct is_same_d {};
}

TEST(traits, is_not_same) {
    ASSERT_TRUE((hud::is_not_same_v<i32, const i32>));
    ASSERT_FALSE((hud::is_not_same_v<i32, hud_test::integer_type>));

    ASSERT_FALSE((hud::is_not_same_v<hud_test::a, hud_test::a>));
    ASSERT_TRUE((hud::is_not_same_v<const hud_test::a, hud_test::a>));
    ASSERT_TRUE((hud::is_not_same_v<volatile hud_test::a, hud_test::a>));
    ASSERT_TRUE((hud::is_not_same_v<const volatile hud_test::a, hud_test::a>));

    ASSERT_TRUE((hud::is_not_same_v<hud_test::a, hud_test::b>));
    ASSERT_TRUE((hud::is_not_same_v<hud_test::a, hud_test::c>));
    ASSERT_FALSE((hud::is_not_same_v<hud_test::a, hud_test::d>));
    ASSERT_FALSE((hud::is_not_same_v<hud_test::c, hud_test::c>));
    ASSERT_FALSE((hud::is_not_same_v<hud_test::is_same_d<hud_test::a>, hud_test::is_same_d<hud_test::a>>));
    ASSERT_TRUE((hud::is_not_same_v<hud_test::is_same_d<hud_test::a>, hud_test::is_same_d<hud_test::c>>));
    ASSERT_TRUE((hud::is_not_same_v<hud_test::is_same_d<hud_test::a>, hud_test::is_same_d<hud_test::b>>));
}