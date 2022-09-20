#include <core/atomics.h>
#include <atomic>

TEST(Atomics, thread_fence)
{

    // Test nothong, just be sure it compile
    Atomics::thread_fence(Atomics::EMemoryOrder::relaxed);
    Atomics::thread_fence(Atomics::EMemoryOrder::consume);
    Atomics::thread_fence(Atomics::EMemoryOrder::acquire);
    Atomics::thread_fence(Atomics::EMemoryOrder::release);
    Atomics::thread_fence(Atomics::EMemoryOrder::acq_rel);
    Atomics::thread_fence(Atomics::EMemoryOrder::seq_cst);
}

TEST(Atomics, load_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {

        alignas(sizeof(type_t)) type_t storage = static_cast<type_t>(32);

        // Atomics::EMemoryOrder::release and Atomics::EMemoryOrder::acq_rel are not valid memory order for load
        ASSERT_EQ(Atomics::load(storage), static_cast<type_t>(32));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::relaxed), static_cast<type_t>(32));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::consume), static_cast<type_t>(32));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::acquire), static_cast<type_t>(32));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::seq_cst), static_cast<type_t>(32));
    });
}

TEST(Atomics, load_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {

        type_t* storage = reinterpret_cast<type_t*>(0x32323232);

        // Atomics::EMemoryOrder::release and Atomics::EMemoryOrder::acq_rel are not valid memory order for load
        ASSERT_EQ(Atomics::load(storage), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(Atomics::load(storage, Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(0x32323232));
    });
}

TEST(Atomics, store_integral)
{

    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{};
        alignas(sizeof(type_t)) type_t storage = default_value;

        // Be sure we really test different value
        ASSERT_EQ(storage, default_value);
        ASSERT_NE(storage, value);

        Atomics::store(storage, value);
        ASSERT_EQ(storage, value);
        storage = default_value;

        Atomics::store(storage, value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(storage, value);
        storage = default_value;

        Atomics::store(storage, value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(storage, value);
        storage = default_value;

        Atomics::store(storage, value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(storage, value);
    });
}


TEST(Atomics, store_pointer)
{

    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const default_value = nullptr;
        type_t* storage = nullptr;

        // Be sure we really test different value
        ASSERT_EQ(storage, default_value);
        ASSERT_NE(storage, value);

        Atomics::store(storage, value);
        ASSERT_EQ(storage, reinterpret_cast<type_t*>(0x32323232));
        storage = default_value;

        Atomics::store(storage, value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(storage, reinterpret_cast<type_t*>(0x32323232));
        storage = default_value;

        Atomics::store(storage, value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(storage, reinterpret_cast<type_t*>(0x32323232));
        storage = default_value;

        Atomics::store(storage, value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(storage, reinterpret_cast<type_t*>(0x32323232));
    });
}

TEST(Atomics, compare_and_swap_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        constexpr const type_t expected = type_t{};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::relaxed), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::relaxed), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::consume), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::consume), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acquire), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acquire), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::release), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::release), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acq_rel), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acq_rel), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::seq_cst), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::seq_cst), destination);
        ASSERT_EQ(destination, value);
    });
}


TEST(Atomics, compare_and_swap_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        constexpr type_t* const expected = nullptr;
        type_t* destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::relaxed), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::relaxed), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::consume), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::consume), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acquire), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acquire), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::release), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::release), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acq_rel), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::acq_rel), destination);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        // Success because destination == expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::seq_cst), expected);
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_EQ(Atomics::compare_and_swap(destination, expected, value, Atomics::EMemoryOrder::seq_cst), destination);
        ASSERT_EQ(destination, value);
    });
}

TEST(Atomics, compare_and_set_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        type_t expected = type_t{};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = type_t{};

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(destination, value);
    });
}

TEST(Atomics, compare_and_set_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* expected = nullptr;
        type_t* destination = destination_default_value;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(destination, value);
        destination = destination_default_value;
        expected = nullptr;

        // Success because destination == expected
        ASSERT_TRUE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(destination, value);
        // Failed because destination != expected
        ASSERT_FALSE(Atomics::compare_and_set(destination, expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(destination, value);
    });
}

TEST(Atomics, exchange_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(destination, value);
    });
}

TEST(Atomics, exchange_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(destination, value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::exchange(destination, value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(destination, value);
    });
}

TEST(Atomics, fetch_add_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{16};
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
    });
}

TEST(Atomics, fetch_add_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const isize value = 32;
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_add(destination, value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(destination, destination_default_value + value);
    });
}

TEST(Atomics, fetch_sub_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{ 16 };
        alignas(sizeof(type_t)) type_t destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value), destination_default_value);
        ASSERT_EQ(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(destination, static_cast<type_t>(destination_default_value - value));
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(destination, static_cast<type_t>(destination_default_value - value));
    });
}

TEST(Atomics, fetch_sub_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const isize value = 32;
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value), destination_default_value);
        ASSERT_EQ(destination, destination_default_value - value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(destination, destination_default_value - value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(destination, destination_default_value - value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(destination, destination_default_value - value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(destination, destination_default_value - value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(destination, destination_default_value - value);
        destination = destination_default_value;

        ASSERT_EQ(Atomics::fetch_sub(destination, value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(destination, destination_default_value - value);
    });
}

TEST(Atomic, default_constructor_of_integral_do_not_initialize_atomic)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        alignas(sizeof(type_t)) u8 a[sizeof(Atomic<type_t>)];
        hud::memory::set(a, u8_max);

        Atomic<type_t>* atomic = new (a) Atomic<type_t>;
        ASSERT_NE(atomic->load(), type_t{});
    });
}

TEST(Atomic, default_constructor_of_pointer_do_not_initialize_atomic)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        alignas(type_t*) u8 a[sizeof(Atomic<type_t*>)];
        hud::memory::set(a, u8_max);

        Atomic<type_t*>* atomic = new (a) Atomic<type_t*>;
        ASSERT_NE(atomic->load(), nullptr);
    });
}

TEST(Atomic, constructor_of_integral_initialize_atomic_to_value)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        Atomic<type_t> a(32);
        ASSERT_EQ(a.load(), type_t{ 32 });
    });
}

TEST(Atomic, constructor_of_pointer_with_tag_initialize_atomic_to_value)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        Atomic<type_t*> a(reinterpret_cast<type_t*>(0x32));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(0x32));
    });
}

TEST(Atomic, cast_to_type_load_and_return_the_integral_value)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        Atomic<type_t> a(32);
        ASSERT_EQ(static_cast<type_t>(a), type_t{ 32 });
    });
}

TEST(Atomic, cast_to_type_load_and_return_the_pointer_value)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        Atomic<type_t*> a(reinterpret_cast<type_t*>(0x32));
        ASSERT_EQ(static_cast<type_t*>(a), reinterpret_cast<type_t*>(0x32));
    });
}

TEST(Atomic, volatile_cast_to_type_load_and_return_the_integral_value)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        volatile Atomic<type_t> a(32);
        ASSERT_EQ(static_cast<type_t>(a), type_t{ 32 });
    });
}

TEST(Atomic, volatile_cast_to_type_load_and_return_the_pointer_value)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        volatile Atomic<type_t*> a(reinterpret_cast<type_t*>(0x32));
        ASSERT_EQ(static_cast<type_t*>(a), reinterpret_cast<type_t*>(0x32));
    });
}

TEST(Atomic, assign_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        Atomic<type_t> a(0);
        ASSERT_EQ(a.load(), type_t{ 0 });
        a = 32;
        ASSERT_EQ(a.load(), type_t{ 32 });
    });
}

TEST(Atomic, assign_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        Atomic<type_t*> a(nullptr);
        ASSERT_EQ(a.load(), nullptr);
        a = reinterpret_cast<type_t*>(0x32);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(0x32));
    });
}

TEST(Atomic, volatile_assign_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        volatile Atomic<type_t> a(0);
        ASSERT_EQ(a.load(), type_t{ 0 });
        a = 32;
        ASSERT_EQ(a.load(), type_t{ 32 });
    });
}

TEST(Atomic, volatile_assign_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        volatile Atomic<type_t*> a(nullptr);
        ASSERT_EQ(a.load(), nullptr);
        a = reinterpret_cast<type_t*>(0x32);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(0x32));
    });
}


TEST(Atomic, store_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 0 };

        Atomic<type_t> a(default_value);

        // Be sure we really test different value
        ASSERT_EQ(a.load(), default_value);

        a.store(value);
        ASSERT_EQ(a.load(), value);
        a.store(default_value);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), default_value);
    });
}

TEST(Atomic, store_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const value = reinterpret_cast<type_t*>( 0x32);
        type_t* const default_value = nullptr;

        Atomic<type_t*> a(default_value);

        // Be sure we really test different value
        ASSERT_EQ(a.load(), default_value);

        a.store(value);
        ASSERT_EQ(a.load(), value);
        a.store(default_value);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), default_value);
    });
}

TEST(Atomic, volatile_store_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 0 };

        volatile Atomic<type_t> a(default_value);

        // Be sure we really test different value
        ASSERT_EQ(a.load(), default_value);

        a.store(value);
        ASSERT_EQ(a.load(), value);
        a.store(default_value);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), default_value);
    });
}

TEST(Atomic, volatile_store_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const value = reinterpret_cast<type_t*>(0x32);
        type_t* const default_value = nullptr;

        volatile Atomic<type_t*> a(default_value);

        // Be sure we really test different value
        ASSERT_EQ(a.load(), default_value);

        a.store(value);
        ASSERT_EQ(a.load(), value);
        a.store(default_value);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::relaxed);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::release);
        ASSERT_EQ(a.load(), default_value);

        a.store(value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), value);
        a.store(default_value, Atomics::EMemoryOrder::seq_cst);
        ASSERT_EQ(a.load(), default_value);
    });
}

TEST(Atomic, load_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {

        Atomic<type_t> a(type_t{ 32 });

        // Atomics::EMemoryOrder::release and Atomics::EMemoryOrder::acq_rel are not valid memory order for load
        ASSERT_EQ(a.load(), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::relaxed), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::consume), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::acquire), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::seq_cst), type_t{ 32 });
    });
}

TEST(Atomic, load_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {

        Atomic<type_t*> a(reinterpret_cast<type_t*>(0x32323232));

        // Atomics::EMemoryOrder::release and Atomics::EMemoryOrder::acq_rel are not valid memory order for load
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(0x32323232));
    });
}

TEST(Atomic, volatile_load_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {

        volatile Atomic<type_t> a(type_t{ 32 });

        // Atomics::EMemoryOrder::release and Atomics::EMemoryOrder::acq_rel are not valid memory order for load
        ASSERT_EQ(a.load(), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::relaxed), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::consume), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::acquire), type_t{ 32 });
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::seq_cst), type_t{ 32 });
    });
}

TEST(Atomic, volatile_load_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {

        volatile Atomic<type_t*> a(reinterpret_cast<type_t*>(0x32323232));

        // Atomics::EMemoryOrder::release and Atomics::EMemoryOrder::acq_rel are not valid memory order for load
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(0x32323232));
        ASSERT_EQ(a.load(Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(0x32323232));
    });
}


TEST(Atomic, exchange_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        Atomic<type_t> a (destination_default_value);

        ASSERT_EQ(a.exchange(value), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(a.load(), value);
    });
}

TEST(Atomic, exchange_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        Atomic<type_t*> a(destination_default_value);

        ASSERT_EQ(a.exchange(value), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(a.load(), value);
    });
}

TEST(Atomic, volatile_exchange_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t destination_default_value = type_t{};
        volatile Atomic<type_t> a(destination_default_value);

        ASSERT_EQ(a.exchange(value), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(a.load(), value);
    });
}

TEST(Atomic, volatile_exchange_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        volatile Atomic<type_t*> a(destination_default_value);

        ASSERT_EQ(a.exchange(value), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::relaxed), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::consume), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acquire), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::release), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::acq_rel), destination_default_value);
        ASSERT_EQ(a.load(), value);
        a = destination_default_value;

        ASSERT_EQ(a.exchange(value, Atomics::EMemoryOrder::seq_cst), destination_default_value);
        ASSERT_EQ(a.load(), value);
    });
}

TEST(Atomic, compare_exchange_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        type_t destination_default_value = type_t{};
        Atomic<type_t> a(destination_default_value);

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
    });
}


TEST(Atomic, compare_exchange_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* expected = nullptr;
        Atomic<type_t*> a(destination_default_value);

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
    });
}

TEST(Atomic, volatile_compare_exchange_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        type_t destination_default_value = type_t{};
        volatile Atomic<type_t> a(destination_default_value);

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
        destination_default_value = type_t{};
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(destination_default_value, type_t{});
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(destination_default_value, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(destination_default_value, value);
    });
}


TEST(Atomic, volatile_compare_exchange_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        type_t* const  value = reinterpret_cast<type_t*>(0x32323232);
        constexpr type_t* const  destination_default_value = nullptr;
        type_t* expected = nullptr;
        volatile Atomic<type_t*> a(destination_default_value);

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::relaxed));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::consume));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acquire));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::release));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::acq_rel));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
        expected = destination_default_value;
        a = destination_default_value;

        // Success because a == destination_default_value
        ASSERT_TRUE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange success, the expected value should not be modified
        ASSERT_EQ(expected, nullptr);
        // Failed because a != destination_default_value
        ASSERT_FALSE(a.compare_exchange(expected, value, Atomics::EMemoryOrder::seq_cst));
        ASSERT_EQ(a.load(), value);
        // If exchange failed, the expected value should be set to the atomic stored value
        ASSERT_EQ(expected, value);
    });
}

TEST(Atomic, add_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.add(value), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::relaxed), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::consume), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acquire), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::release), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acq_rel), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::seq_cst), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}

TEST(Atomic, volatile_add_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a.add(value), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::relaxed), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::consume), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acquire), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::release), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acq_rel), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::seq_cst), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}

TEST(Atomic, fetch_add_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_add(value), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}

TEST(Atomic, volatile_fetch_add_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_add(value), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}

TEST(Atomic, subtract_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.subtract(value), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::relaxed), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::consume), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acquire), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::release), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acq_rel), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::seq_cst), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
    });
}

TEST(Atomic, volatile_subtract_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a.subtract(value), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::relaxed), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::consume), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acquire), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::release), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acq_rel), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::seq_cst), static_cast<type_t>(default_value - value));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
    });
}

TEST(Atomic, fetch_sub_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_sub(value), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
    });
}

TEST(Atomic, volatile_fetch_sub_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t value = type_t{ 32 };
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_sub(value), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - value));
    });
}

TEST(Atomic, increment_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.increment(), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::relaxed), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::consume), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acquire), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::release), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acq_rel), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::seq_cst), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, volatile_increment_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a.increment(), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::relaxed), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::consume), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acquire), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::release), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acq_rel), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::seq_cst), default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, fetch_increment_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_increment(), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, volatile_fetch_increment_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_increment(), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, decrement_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.decrement(), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::relaxed), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::consume), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acquire), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::release), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acq_rel), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::seq_cst), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, volatile_decrement_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.decrement(), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::relaxed), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::consume), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acquire), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::release), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acq_rel), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::seq_cst), static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, fetch_decrement_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_decrement(), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, volatile_fetch_decrement_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a.fetch_decrement(), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, pre_increment_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(++a, default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, volatile_pre_increment_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(++a, default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}


TEST(Atomic, post_increment_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a++, default_value);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, volatile_post_increment_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a++, default_value);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, pre_decrement_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(--a, static_cast<type_t>(default_value -1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, volatile_pre_decrement_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(--a, static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, post_decrement_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a--, default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, volatile_post_decrement_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a--, default_value);
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, increment_equal_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a+=1, default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, volatile_increment_equal_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a+=1, default_value + 1);
        ASSERT_EQ(a.load(), default_value + 1);
    });
}

TEST(Atomic, decrement_equal_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        Atomic<type_t> a(default_value);

        ASSERT_EQ(a-=1, static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}

TEST(Atomic, volatile_decrement_equal_operator_integral)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr const type_t default_value = type_t{ 16 };
        volatile Atomic<type_t> a(default_value);

        ASSERT_EQ(a-=1, static_cast<type_t>(default_value - 1));
        ASSERT_EQ(a.load(), static_cast<type_t>(default_value - 1));
    });
}


TEST(Atomic, add_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t* const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.add(value), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::relaxed), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::consume), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acquire), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::release), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acq_rel), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::seq_cst), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}

TEST(Atomic, volatile_add_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.add(value), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::relaxed), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::consume), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acquire), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::release), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::acq_rel), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.add(value, Atomics::EMemoryOrder::seq_cst), default_value + value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}

TEST(Atomic, fetch_add_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_add(value), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}

TEST(Atomic, volatile_fetch_add_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_add(value), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), default_value + value);
        a = default_value;

        ASSERT_EQ(a.fetch_add(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), default_value + value);
    });
}


TEST(Atomic, subtract_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.subtract(value), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::release), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acq_rel), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
    });
}

TEST(Atomic, volatile_subtract_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t*const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.subtract(value), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::release), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::acq_rel), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.subtract(value, Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(default_value - value));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
    });
}

TEST(Atomic, fetch_sub_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t* const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_sub(value), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
    });
}


TEST(Atomic, volatile_fetch_sub_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        const isize value = 32;
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_sub(value), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
        a = default_value;

        ASSERT_EQ(a.fetch_sub(value, Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - value));
    });
}

TEST(Atomic, increment_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.increment(), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::release), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acq_rel), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, volatile_increment_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t*const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.increment(), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::release), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::acq_rel), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.increment(Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, fetch_increment_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_increment(), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, volatile_fetch_increment_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_increment(), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
        a = default_value;

        ASSERT_EQ(a.fetch_increment(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, decrement_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t*const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.decrement(), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::release), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acq_rel), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}

TEST(Atomic, volatile_decrement_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.decrement(), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::relaxed), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::consume), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acquire), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::release), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::acq_rel), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.decrement(Atomics::EMemoryOrder::seq_cst), reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}

TEST(Atomic, fetch_decrement_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_decrement(), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}

TEST(Atomic, volatile_fetch_decrement_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a.fetch_decrement(), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::relaxed), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::consume), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acquire), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::release), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::acq_rel), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
        a = default_value;

        ASSERT_EQ(a.fetch_decrement(Atomics::EMemoryOrder::seq_cst), default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}

TEST(Atomic, pre_increment_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t*const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(++a, reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, volatile_pre_increment_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(++a, reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, post_increment_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a++, default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, volatile_post_increment_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a++, default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, pre_decrement_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(--a, reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}


TEST(Atomic, volatile_pre_decrement_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(--a, reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}

TEST(Atomic, post_decrement_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a--, default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}

TEST(Atomic, volatile_post_decrement_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a--, default_value);
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}


TEST(Atomic, increment_equal_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t*const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a += 1, reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, volatile_increment_equal_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t* const default_value = nullptr;
        volatile Atomic<type_t*> a(default_value);

        ASSERT_EQ(a += 1, reinterpret_cast<type_t*>(default_value + 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value + 1));
    });
}

TEST(Atomic, decrement_equal_operator_pointer)
{


    hud::test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>() {
        constexpr type_t*const default_value = nullptr;
        Atomic<type_t*> a(default_value);

        ASSERT_EQ(a -= 1, reinterpret_cast<type_t*>(default_value - 1));
        ASSERT_EQ(a.load(), reinterpret_cast<type_t*>(default_value - 1));
    });
}