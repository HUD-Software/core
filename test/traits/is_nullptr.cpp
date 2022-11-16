#include <core/traits/is_nullptr.h>

namespace hud_test {
    [[maybe_unused]] int* a = 0;
}

GTEST_TEST(traits, is_nullptr) {
    ASSERT_FALSE((hud::is_nullptr_v<decltype(hud_test::a)>));
    ASSERT_FALSE((hud::is_nullptr_v<decltype(0)>));
    ASSERT_FALSE((hud::is_nullptr_v<decltype(&(*hud_test::a))>));
    ASSERT_TRUE((hud::is_nullptr_v<decltype(nullptr)>));
    ASSERT_TRUE((hud::is_nullptr_v<std::nullptr_t>));
    ASSERT_TRUE((hud::is_nullptr_v<hud::ptr::null>));
}