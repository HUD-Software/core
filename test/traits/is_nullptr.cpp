#include <core/traits/is_nullptr.h>

namespace hud_test {
    [[maybe_unused]] int* a = 0;
}

GTEST_TEST(traits, is_nullptr) {
    GTEST_ASSERT_FALSE((hud::is_nullptr_v<decltype(hud_test::a)>));
    GTEST_ASSERT_FALSE((hud::is_nullptr_v<decltype(0)>));
    GTEST_ASSERT_FALSE((hud::is_nullptr_v<decltype(&(*hud_test::a))>));
    GTEST_ASSERT_TRUE((hud::is_nullptr_v<decltype(nullptr)>));
    GTEST_ASSERT_TRUE((hud::is_nullptr_v<std::nullptr_t>));
    GTEST_ASSERT_TRUE((hud::is_nullptr_v<hud::ptr::null>));
}