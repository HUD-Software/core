#include <core/atomics.h>

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