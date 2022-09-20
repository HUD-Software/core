#include <core/traits/is_not_same.h>

namespace {
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
    ASSERT_FALSE((hud::is_not_same_v<i32, integer_type>));

    ASSERT_FALSE((hud::is_not_same_v<a, a>));
    ASSERT_TRUE((hud::is_not_same_v<const a, a>));
    ASSERT_TRUE((hud::is_not_same_v<volatile a, a>));
    ASSERT_TRUE((hud::is_not_same_v<const volatile a, a>));

    ASSERT_TRUE((hud::is_not_same_v<a, b>));
    ASSERT_TRUE((hud::is_not_same_v<a, c>));
    ASSERT_FALSE((hud::is_not_same_v<a, d>));
    ASSERT_FALSE((hud::is_not_same_v<c, c>));
    ASSERT_FALSE((hud::is_not_same_v<is_same_d<a>, is_same_d<a>>));
    ASSERT_TRUE((hud::is_not_same_v<is_same_d<a>, is_same_d<c>>));
    ASSERT_TRUE((hud::is_not_same_v<is_same_d<a>, is_same_d<b>>));
}