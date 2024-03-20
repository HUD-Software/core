#include <core/templates/exchange.h>

GTEST_TEST(templates, exchange_trivial_same_type)
{

    i32 a = 5;
    i32 b = 32;
    i32 old = hud::exchange(a, b);

    hud_assert_eq(a, 32);
    hud_assert_eq(b, 32);
    hud_assert_eq(old, 5);
}

GTEST_TEST(templates, exchange_trivial_different_type)
{

    i32 a = 5;
    u32 b = 32;
    i32 old = hud::exchange(a, b);

    hud_assert_eq(a, 32);
    hud_assert_eq(b, 32);
    hud_assert_eq(old, 5);
}

GTEST_TEST(templates, exchange_non_trivial_move_assignable_same_type)
{
    // When new_value is l-reference
    {
        hud_test::non_bitwise_move_assignable_type t(2);
        hud_test::non_bitwise_move_assignable_type t1(3);
        hud_test::non_bitwise_move_assignable_type old = hud::exchange(t, t1);

        hud_assert_eq(t.id(), 3);
        hud_assert_eq(t.copy_assign_count(), 1u);
        hud_assert_eq(t.move_assign_count(), 0u);
        hud_assert_eq(t.copy_constructor_count(), 0u);
        hud_assert_eq(t.move_constructor_count(), 0u);
        hud_assert_eq(t1.id(), 3);
        hud_assert_eq(t1.copy_assign_count(), 0u);
        hud_assert_eq(t1.move_assign_count(), 0u);
        hud_assert_eq(t1.copy_constructor_count(), 0u);
        hud_assert_eq(t1.move_constructor_count(), 0u);
        hud_assert_eq(old.id(), 2);
        hud_assert_eq(old.copy_assign_count(), 0u);
        hud_assert_eq(old.move_assign_count(), 0u);
        hud_assert_eq(old.copy_constructor_count(), 0u);
        hud_assert_eq(old.move_constructor_count(), 1u);
    }

    // When new_value is r-reference
    {
        hud_test::non_bitwise_move_assignable_type t(2);
        hud_test::non_bitwise_move_assignable_type t1(3);
        hud_test::non_bitwise_move_assignable_type old = hud::exchange(t, hud::move(t1));

        hud_assert_eq(t.id(), 3);
        hud_assert_eq(t.copy_assign_count(), 0u);
        hud_assert_eq(t.move_assign_count(), 1u);
        hud_assert_eq(t.copy_constructor_count(), 0u);
        hud_assert_eq(t.move_constructor_count(), 0u);
        hud_assert_eq(t1.id(), 3);
        hud_assert_eq(t1.copy_assign_count(), 0u);
        hud_assert_eq(t1.move_assign_count(), 0u);
        hud_assert_eq(t1.copy_constructor_count(), 0u);
        hud_assert_eq(t1.move_constructor_count(), 0u);
        hud_assert_eq(old.id(), 2);
        hud_assert_eq(old.copy_assign_count(), 0u);
        hud_assert_eq(old.move_assign_count(), 0u);
        hud_assert_eq(old.copy_constructor_count(), 0u);
        hud_assert_eq(old.move_constructor_count(), 1u);
    }
}

GTEST_TEST(templates, exchange_non_trivial_move_assignable_different_type)
{
    // When new_value is l-reference
    {
        hud_test::non_bitwise_move_assignable_type t(2);
        hud_test::non_bitwise_move_assignable_type2 t1(3);
        hud_test::non_bitwise_move_assignable_type old = hud::exchange(t, t1);

        hud_assert_eq(t.id(), 3);
        hud_assert_eq(t.copy_assign_count(), 1u);
        hud_assert_eq(t.move_assign_count(), 0u);
        hud_assert_eq(t.copy_constructor_count(), 0u);
        hud_assert_eq(t.move_constructor_count(), 0u);
        hud_assert_eq(t1.id(), 3);
        hud_assert_eq(t1.copy_assign_count(), 0u);
        hud_assert_eq(t1.move_assign_count(), 0u);
        hud_assert_eq(t1.copy_constructor_count(), 0u);
        hud_assert_eq(t1.move_constructor_count(), 0u);
        hud_assert_eq(old.id(), 2);
        hud_assert_eq(old.copy_assign_count(), 0u);
        hud_assert_eq(old.move_assign_count(), 0u);
        hud_assert_eq(old.copy_constructor_count(), 0u);
        hud_assert_eq(old.move_constructor_count(), 1u);
    }

    // When new_value is r-reference
    {
        hud_test::non_bitwise_move_assignable_type t(2);
        hud_test::non_bitwise_move_assignable_type2 t1(3);
        hud_test::non_bitwise_move_assignable_type old = hud::exchange(t, hud::move(t1));

        hud_assert_eq(t.id(), 3);
        hud_assert_eq(t.copy_assign_count(), 0u);
        hud_assert_eq(t.move_assign_count(), 1u);
        hud_assert_eq(t.copy_constructor_count(), 0u);
        hud_assert_eq(t.move_constructor_count(), 0u);
        hud_assert_eq(t1.id(), 3);
        hud_assert_eq(t1.copy_assign_count(), 0u);
        hud_assert_eq(t1.move_assign_count(), 0u);
        hud_assert_eq(t1.copy_constructor_count(), 0u);
        hud_assert_eq(t1.move_constructor_count(), 0u);
        hud_assert_eq(old.id(), 2);
        hud_assert_eq(old.copy_assign_count(), 0u);
        hud_assert_eq(old.move_assign_count(), 0u);
        hud_assert_eq(old.copy_constructor_count(), 0u);
        hud_assert_eq(old.move_constructor_count(), 1u);
    }
}
