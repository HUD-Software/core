#include <core/templates/forward.h>
#include <core/traits/is_rvalue_reference.h>

GTEST_TEST(templates, forward)
{

    GTEST_ASSERT_TRUE(hud::is_rvalue_reference_v<decltype(hud::forward<i32>(i32{}))>);
    GTEST_ASSERT_FALSE(hud::is_lvalue_reference_v<decltype(hud::forward<i32>(i32{}))>);

    GTEST_ASSERT_TRUE(hud::is_rvalue_reference_v<decltype(hud::forward<const i32>(i32{}))>);
    GTEST_ASSERT_FALSE(hud::is_lvalue_reference_v<decltype(hud::forward<const i32>(i32{}))>);
}