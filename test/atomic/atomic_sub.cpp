#include <core/atomics.h>

GTEST_TEST(atomic, subtract_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.subtract(value), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::relaxed), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::consume), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acquire), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::release), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acq_rel), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::seq_cst), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value)); });
}

GTEST_TEST(atomic, volatile_subtract_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.subtract(value), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::relaxed), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::consume), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acquire), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::release), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acq_rel), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::seq_cst), static_cast<type_t>(default_value - value));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value)); });
}

GTEST_TEST(atomic, fetch_sub_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_sub(value), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value)); });
}

GTEST_TEST(atomic, volatile_fetch_sub_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_sub(value), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - value)); });
}

GTEST_TEST(atomic, subtract_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.subtract(value), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::relaxed), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::consume), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acquire), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::release), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acq_rel), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::seq_cst), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value)); });
}

GTEST_TEST(atomic, volatile_subtract_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.subtract(value), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::relaxed), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::consume), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acquire), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::release), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acq_rel), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::seq_cst), reinterpret_cast<const type_t*>(destination_default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value)); });
}

GTEST_TEST(atomic, fetch_sub_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.fetch_sub(value), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value)); });
}

GTEST_TEST(atomic, volatile_fetch_sub_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.fetch_sub(value), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value));
        a = destination_default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - value)); });
}