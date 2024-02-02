#include <core/atomics.h>

GTEST_TEST(atomic, exchange_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        hud::atomic<type_t> a (destination_default_value);

        hud_assert_eq(a.exchange(value), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), value); });
}

GTEST_TEST(atomic, exchange_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        hud::atomic<type_t*> a(destination_default_value);

        hud_assert_eq(a.exchange(value), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), value); });
}

GTEST_TEST(atomic, volatile_exchange_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        volatile hud::atomic<type_t> a(destination_default_value);

        hud_assert_eq(a.exchange(value), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), value); });
}

GTEST_TEST(atomic, volatile_exchange_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        volatile hud::atomic<type_t*> a(destination_default_value);

        hud_assert_eq(a.exchange(value), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), value);
        a = destination_default_value;

        hud_assert_eq(a.exchange(value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), value); });
}