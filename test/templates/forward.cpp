#include <core/templates/forward.h>
#include <core/traits/is_rvalue_reference.h>

GTEST_TEST(templates, forward)
{

    hud_assert_true(hud::is_rvalue_reference_v<decltype(hud::forward<i32>(i32 {}))>);
    hud_assert_false(hud::is_lvalue_reference_v<decltype(hud::forward<i32>(i32 {}))>);

    hud_assert_true(hud::is_rvalue_reference_v<decltype(hud::forward<const i32>(i32 {}))>);
    hud_assert_false(hud::is_lvalue_reference_v<decltype(hud::forward<const i32>(i32 {}))>);
}