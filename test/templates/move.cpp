#include <core/templates/move.h>

GTEST_TEST(templates, move)
{
    i32 a {};
    hud_assert_true(hud::is_rvalue_reference_v<decltype(hud::move(a))>);
    hud_assert_false(hud::is_lvalue_reference_v<decltype(hud::move(a))>);

    const i32 a_const {};
    hud_assert_true(hud::is_rvalue_reference_v<decltype(hud::move(a_const))>);
    hud_assert_false(hud::is_lvalue_reference_v<decltype(hud::move(a_const))>);
}