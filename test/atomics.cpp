#include <core/atomics.h>
#include <atomic>

GTEST_TEST(atomics, thread_fence)
{

    // Test nothong, just be sure it compile
    hud::atomics::thread_fence(hud::atomics::memory_order_e::relaxed);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::consume);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::acquire);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::release);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::acq_rel);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::seq_cst);
}

GTEST_TEST(atomics, load_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {

        alignas(sizeof(type_t)) type_t storage = static_cast<type_t>(32);

        // hud::atomics::memory_order_e::release and hud::atomics::memory_order_e::acq_rel are not valid memory order for load
        hud_assert_eq(hud::atomics::load(storage), static_cast<type_t>(32));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::relaxed), static_cast<type_t>(32));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::consume), static_cast<type_t>(32));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::acquire), static_cast<type_t>(32));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::seq_cst), static_cast<type_t>(32)); });
}

GTEST_TEST(atomics, load_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {

        type_t* storage = reinterpret_cast<type_t*>(0x32323232);

        // hud::atomics::memory_order_e::release and hud::atomics::memory_order_e::acq_rel are not valid memory order for load
        hud_assert_eq(hud::atomics::load(storage), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(hud::atomics::load(storage, hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(0x32323232)); });
}

GTEST_TEST(atomics, store_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{};
        alignas(sizeof(type_t)) type_t storage = default_value;

        // Be sure we really test different value
        hud_assert_eq(storage, default_value);
        hud_assert_ne(storage, value);

        hud::atomics::store(storage, value);
        hud_assert_eq(storage, value);
        storage = default_value;

        hud::atomics::store(storage, value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(storage, value);
        storage = default_value;

        hud::atomics::store(storage, value, hud::atomics::memory_order_e::release);
        hud_assert_eq(storage, value);
        storage = default_value;

        hud::atomics::store(storage, value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(storage, value); });
}

GTEST_TEST(atomics, store_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const default_value = nullptr;
        type_t* storage = nullptr;

        // Be sure we really test different value
        hud_assert_eq(storage, default_value);
        hud_assert_ne(storage, value);

        hud::atomics::store(storage, value);
        hud_assert_eq(storage, reinterpret_cast<type_t*>(0x32323232));
        storage = default_value;

        hud::atomics::store(storage, value, hud::atomics::memory_order_e::relaxed);
        hud_assert_eq(storage, reinterpret_cast<type_t*>(0x32323232));
        storage = default_value;

        hud::atomics::store(storage, value, hud::atomics::memory_order_e::release);
        hud_assert_eq(storage, reinterpret_cast<type_t*>(0x32323232));
        storage = default_value;

        hud::atomics::store(storage, value, hud::atomics::memory_order_e::seq_cst);
        hud_assert_eq(storage, reinterpret_cast<type_t*>(0x32323232)); });
}

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

GTEST_TEST(atomics, exchange_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(destination, value); });
}

GTEST_TEST(atomics, exchange_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(destination, value);
        destination = destination_default_value;

        hud_assert_eq(hud::atomics::exchange(destination, value, hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(destination, value); });
}

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
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* destination = destination_default_value;

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
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* destination = destination_default_value;

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

GTEST_TEST(atomic, default_constructor_of_integral_do_not_initialize_atomic)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        alignas(sizeof(type_t)) u8 a[sizeof(hud::atomic<type_t>)];
        hud::memory::set(a, hud::u8_max);

        hud::atomic<type_t>* atomic = new (a) hud::atomic<type_t>;
        hud_assert_ne(atomic->load(), type_t{}); });
}

GTEST_TEST(atomic, default_constructor_of_pointer_do_not_initialize_atomic)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        alignas(type_t*) u8 a[sizeof(hud::atomic<type_t*>)];
        hud::memory::set(a, hud::u8_max);

        hud::atomic<type_t*>* atomic = new (a) hud::atomic<type_t*>;
        hud_assert_ne(atomic->load(), (type_t*)nullptr); });
}

GTEST_TEST(atomic, constructor_of_integral_initialize_atomic_to_value)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        hud::atomic<type_t> a(32);
        hud_assert_eq( a.load(), type_t{ 32 }); });
}

GTEST_TEST(atomic, constructor_of_pointer_with_tag_initialize_atomic_to_value)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        hud::atomic<type_t*> a(reinterpret_cast<type_t*>(0x32));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(0x32)); });
}

GTEST_TEST(atomic, cast_to_type_load_and_return_the_integral_value)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        hud::atomic<type_t> a(32);
        hud_assert_eq(static_cast<type_t>(a), type_t{ 32 }); });
}

GTEST_TEST(atomic, cast_to_type_load_and_return_the_pointer_value)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        hud::atomic<type_t*> a(reinterpret_cast<type_t*>(0x32));
        hud_assert_eq(static_cast<type_t*>(a), reinterpret_cast<type_t*>(0x32)); });
}

GTEST_TEST(atomic, volatile_cast_to_type_load_and_return_the_integral_value)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        volatile hud::atomic<type_t> a(32);
        hud_assert_eq(static_cast<type_t>(a), type_t{ 32 }); });
}

GTEST_TEST(atomic, volatile_cast_to_type_load_and_return_the_pointer_value)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        volatile hud::atomic<type_t*> a(reinterpret_cast<type_t*>(0x32));
        hud_assert_eq(static_cast<type_t*>(a), reinterpret_cast<type_t*>(0x32)); });
}

GTEST_TEST(atomic, assign_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        hud::atomic<type_t> a(0);
        hud_assert_eq(a.load(), type_t{ 0 });
        a = 32;
        hud_assert_eq(a.load(), type_t{ 32 }); });
}

GTEST_TEST(atomic, assign_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        hud::atomic<type_t*> a(nullptr);
        hud_assert_eq(a.load(), nullptr);
        a = reinterpret_cast<type_t*>(0x32);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(0x32)); });
}

GTEST_TEST(atomic, volatile_assign_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        volatile hud::atomic<type_t> a(0);
        hud_assert_eq(a.load(), type_t{ 0 });
        a = 32;
        hud_assert_eq(a.load(), type_t{ 32 }); });
}

GTEST_TEST(atomic, volatile_assign_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        volatile hud::atomic<type_t*> a(nullptr);
        hud_assert_eq(a.load(), nullptr);
        a = reinterpret_cast<type_t*>(0x32);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(0x32)); });
}

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

GTEST_TEST(atomic, load_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {

        hud::atomic<type_t> a(type_t{ 32 });

        // hud::atomics::memory_order_e::release and hud::atomics::memory_order_e::acq_rel are not valid memory order for load
        hud_assert_eq(a.load(), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::relaxed), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::consume), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::acquire), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::seq_cst), type_t{ 32 }); });
}

GTEST_TEST(atomic, load_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {

        hud::atomic<type_t*> a(reinterpret_cast<type_t*>(0x32323232));

        // hud::atomics::memory_order_e::release and hud::atomics::memory_order_e::acq_rel are not valid memory order for load
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(0x32323232)); });
}

GTEST_TEST(atomic, volatile_load_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {

        volatile hud::atomic<type_t> a(type_t{ 32 });

        // hud::atomics::memory_order_e::release and hud::atomics::memory_order_e::acq_rel are not valid memory order for load
        hud_assert_eq(a.load(), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::relaxed), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::consume), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::acquire), type_t{ 32 });
        hud_assert_eq(a.load(hud::atomics::memory_order_e::seq_cst), type_t{ 32 }); });
}

GTEST_TEST(atomic, volatile_load_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {

        volatile hud::atomic<type_t*> a(reinterpret_cast<type_t*>(0x32323232));

        // hud::atomics::memory_order_e::release and hud::atomics::memory_order_e::acq_rel are not valid memory order for load
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(0x32323232));
        hud_assert_eq(a.load(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(0x32323232)); });
}

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

GTEST_TEST(atomic, add_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.add(value), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::relaxed), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::consume), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acquire), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::release), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acq_rel), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::seq_cst), default_value + value);
        hud_assert_eq(a.load(), default_value + value); });
}

GTEST_TEST(atomic, volatile_add_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.add(value), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::relaxed), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::consume), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acquire), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::release), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acq_rel), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::seq_cst), default_value + value);
        hud_assert_eq(a.load(), default_value + value); });
}

GTEST_TEST(atomic, fetch_add_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_add(value), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), default_value + value); });
}

GTEST_TEST(atomic, volatile_fetch_add_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_add(value), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), default_value + value); });
}

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

GTEST_TEST(atomic, increment_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.increment(), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::relaxed), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::consume), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acquire), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::release), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acq_rel), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::seq_cst), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, volatile_increment_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.increment(), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::relaxed), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::consume), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acquire), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::release), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acq_rel), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::seq_cst), default_value + 1);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, fetch_increment_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_increment(), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, volatile_fetch_increment_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_increment(), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), default_value + 1);
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, decrement_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.decrement(), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::relaxed), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::consume), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acquire), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::release), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acq_rel), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::seq_cst), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_decrement_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.decrement(), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::relaxed), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::consume), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acquire), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::release), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acq_rel), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::seq_cst), static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, fetch_decrement_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_decrement(), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_fetch_decrement_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a.fetch_decrement(), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, pre_increment_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(++a, default_value + 1);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, volatile_pre_increment_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(++a, default_value + 1);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, post_increment_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a++, default_value);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, volatile_post_increment_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a++, default_value);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, pre_decrement_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(--a, static_cast<type_t>(default_value -1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_pre_decrement_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(--a, static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, post_decrement_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a--, default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_post_decrement_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a--, default_value);
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, increment_equal_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a+=1, default_value + 1);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, volatile_increment_equal_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a+=1, default_value + 1);
        hud_assert_eq(a.load(), default_value + 1); });
}

GTEST_TEST(atomic, decrement_equal_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        hud::atomic<type_t> a(default_value);

        hud_assert_eq(a-=1, static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_decrement_equal_operator_integral)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr const type_t default_value = type_t{ 16 };
        volatile hud::atomic<type_t> a(default_value);

        hud_assert_eq(a-=1, static_cast<type_t>(default_value - 1));
        hud_assert_eq(a.load(), static_cast<type_t>(default_value - 1)); });
}

GTEST_TEST(atomic, add_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t* const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.add(value), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::relaxed), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::consume), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acquire), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::release), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acq_rel), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::seq_cst), default_value + value);
        hud_assert_eq(a.load(), default_value + value); });
}

GTEST_TEST(atomic, volatile_add_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.add(value), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::relaxed), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::consume), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acquire), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::release), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::acq_rel), default_value + value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.add(value, hud::atomics::memory_order_e::seq_cst), default_value + value);
        hud_assert_eq(a.load(), default_value + value); });
}

GTEST_TEST(atomic, fetch_add_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_add(value), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), default_value + value); });
}

GTEST_TEST(atomic, volatile_fetch_add_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_add(value), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), default_value + value);
        a = default_value;

        hud_assert_eq(a.fetch_add(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), default_value + value); });
}

GTEST_TEST(atomic, subtract_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.subtract(value), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::release), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acq_rel), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value)); });
}

GTEST_TEST(atomic, volatile_subtract_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.subtract(value), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::release), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::acq_rel), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.subtract(value, hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(default_value - value));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value)); });
}

GTEST_TEST(atomic, fetch_sub_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t* const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_sub(value), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value)); });
}

GTEST_TEST(atomic, volatile_fetch_sub_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const isize value = 32;
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_sub(value), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        hud_assert_eq(a.fetch_sub(value, hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - value)); });
}

GTEST_TEST(atomic, increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.increment(), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::release), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, volatile_increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t*const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.increment(), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::release), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, fetch_increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_increment(), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, volatile_fetch_increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_increment(), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t*const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.decrement(), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::release), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.decrement(), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::relaxed), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::consume), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acquire), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::release), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, fetch_decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_decrement(), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_fetch_decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a.fetch_decrement(), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::relaxed), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::consume), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acquire), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::release), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acq_rel), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::seq_cst), default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, pre_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t*const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(++a, reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, volatile_pre_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(++a, reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, post_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a++, default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, volatile_post_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a++, default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, pre_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(--a, reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_pre_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(--a, reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, post_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a--, default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, volatile_post_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a--, default_value);
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}

GTEST_TEST(atomic, increment_equal_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t*const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a += 1, reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, volatile_increment_equal_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t* const default_value = nullptr;
        volatile hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a += 1, reinterpret_cast<type_t*>(default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value + 1)); });
}

GTEST_TEST(atomic, decrement_equal_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        constexpr type_t*const default_value = nullptr;
        hud::atomic<type_t*> a(default_value);

        hud_assert_eq(a -= 1, reinterpret_cast<type_t*>(default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<type_t*>(default_value - 1)); });
}