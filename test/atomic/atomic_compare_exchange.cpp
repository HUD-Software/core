#include <core/atomics.h>

GTEST_TEST(atomic, compare_exchange_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        type_t destination_default_value = type_t{};
        hud::atomic<type_t> a(destination_default_value);

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value); });
}

GTEST_TEST(atomic, compare_exchange_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* expected = nullptr;
        hud::atomic<type_t*> a(destination_default_value);

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value); });
}

GTEST_TEST(atomic, volatile_compare_exchange_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        type_t destination_default_value = type_t{};
        volatile hud::atomic<type_t> a(destination_default_value);

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(destination_default_value, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(destination_default_value, value); });
}

GTEST_TEST(atomic, volatile_compare_exchange_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* expected = nullptr;
        volatile hud::atomic<type_t*> a(destination_default_value);

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::relaxed));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::consume));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acquire));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::release));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::acq_rel));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        hud_assert_true(a.compare_exchange(expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange success, the expected value should not be modified
        hud_assert_eq(expected, nullptr);
        // Failed because a != destination_default_value
        hud_assert_false(a.compare_exchange(expected, value, hud::atomics::memory_order_e::seq_cst));
        hud_assert_eq(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        hud_assert_eq(expected, value); });
}
