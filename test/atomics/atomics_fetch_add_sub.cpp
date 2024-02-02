#include <core/atomics.h>

GTEST_TEST(atomics, fetch_add_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{16};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value); });
}

GTEST_TEST(atomics, fetch_add_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const isize value = 32;
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        const type_t* destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_add(destination, value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(destination, destination_default_value + value); });
}

GTEST_TEST(atomics, fetch_sub_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{ 16 };
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value), destination_default_value);
        hud_assert_eq(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(destination, static_cast<type_t>(destination_default_value - value)); });
}

GTEST_TEST(atomics, fetch_sub_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const isize value = 32;
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        const type_t* destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value), destination_default_value);
        hud_assert_eq(destination, destination_default_value - value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(destination, destination_default_value - value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(destination, destination_default_value - value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(destination, destination_default_value - value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(destination, destination_default_value - value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(destination, destination_default_value - value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::fetch_sub(destination, value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(destination, destination_default_value - value); });
}