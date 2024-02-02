#include <core/atomics.h>

GTEST_TEST(atomics, compare_and_swap_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        constexpr const type_t expected = type_t{};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::relaxed), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::relaxed), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::consume), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::consume), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acquire), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acquire), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::release), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::release), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acq_rel), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acq_rel), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::seq_cst), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::seq_cst), destination);
        hud_assert_eq(destination, value); });
}

GTEST_TEST(atomics, compare_and_swap_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        constexpr type_t* const expected = nullptr;
        type_t* destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::relaxed), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::relaxed), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::consume), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::consume), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acquire), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acquire), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::release), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::release), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acq_rel), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::acq_rel), destination);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::seq_cst), expected);
        hud_assert_eq(destination, value);
        // Failed because destination != expected
        hud_assert_eq(hud::atomics::compare_and_swap(destination, expected, value, hud::atomics::memory_order_e::seq_cst), destination);
        hud_assert_eq(destination, value); });
}