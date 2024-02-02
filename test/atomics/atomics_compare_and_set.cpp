#include <core/atomics.h>

GTEST_TEST(atomics, compare_and_set_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        type_t expected = type_t{};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(destination, value); });
}

GTEST_TEST(atomics, compare_and_set_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* expected = nullptr;
        type_t* destination = destination_default_value;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        hud_assert_true(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_false(hud::atomics::compare_and_set(destination, expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(destination, value); });
}