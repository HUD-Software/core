#include <core/templates/move.h>

GTEST_TEST(templates, move) {
    i32 a{};
    GTEST_ASSERT_TRUE(hud::is_rvalue_reference_v<decltype(hud::move(a))>);
    GTEST_ASSERT_FALSE(hud::is_lvalue_reference_v<decltype(hud::move(a))>);

    const i32 a_const{};
    GTEST_ASSERT_TRUE(hud::is_rvalue_reference_v<decltype(hud::move(a_const))>);
    GTEST_ASSERT_FALSE(hud::is_lvalue_reference_v<decltype(hud::move(a_const))>);
}