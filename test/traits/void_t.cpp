#include <core/traits/void_t.h>

TEST(traits, void_t) {
    ASSERT_TRUE((hud::is_same_v<hud::void_t<>, void>));
}