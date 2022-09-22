#include <core/traits/add_const.h>

TEST(traits, add_const) {
    ASSERT_TRUE((hud::is_same_v<hud::add_const_t<i32>, const i32>));
    ASSERT_TRUE((hud::is_same_v<hud::add_const_t<const i32>, const i32>));
    ASSERT_TRUE((hud::is_same_v<hud::add_const_t<volatile i32>, const volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::add_const_t<i32*>, i32* const>));
    ASSERT_TRUE((hud::is_same_v<hud::add_const_t<i32* volatile>, i32* const volatile>));
}