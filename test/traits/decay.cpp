#include <core/traits/decay.h>

TEST(traits, decay) {
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32&>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32&&>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<const i32&>, i32>));
    ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32[]>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32[]>, i32*>));
    ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32[2]>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32[2]>, i32*>));
    ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32*>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32*>, i32*>));
    ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32(i32)>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32(i32)>, i32(*)(i32)>));
}

