#include <core/atomics.h>

GTEST_TEST(atomic, store_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 0 };

        hud::atomic<type_t> a(default_value);

        // Be sure we really test different value
        hud_assert_eq(a.load(), default_value);

        a.store(value);
        hud_assert_eq(a.load(), value);
        a.store(default_value);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), default_value); });
}

GTEST_TEST(atomic, store_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const value = reinterpret_cast<type_t*>( 0x32);
        type_t* const default_value = nullptr;

        hud::atomic<type_t*> a(default_value);

        // Be sure we really test different value
        hud_assert_eq(a.load(), default_value);

        a.store(value);
        hud_assert_eq(a.load(), value);
        a.store(default_value);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), default_value); });
}

GTEST_TEST(atomic, volatile_store_integral)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 0 };

        volatile hud::atomic<type_t> a(default_value);

        // Be sure we really test different value
        hud_assert_eq(a.load(), default_value);

        a.store(value);
        hud_assert_eq(a.load(), value);
        a.store(default_value);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), default_value); });
}

GTEST_TEST(atomic, volatile_store_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const value = reinterpret_cast<type_t*>(0x32);
        type_t* const default_value = nullptr;

        volatile hud::atomic<type_t*> a(default_value);

        // Be sure we really test different value
        hud_assert_eq(a.load(), default_value);

        a.store(value);
        hud_assert_eq(a.load(), value);
        a.store(default_value);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::release);
        hud_assert_eq(a.load(), default_value);

        a.store(value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), value);
        a.store(default_value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(a.load(), default_value); });
}