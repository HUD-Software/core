#include <core/atomics.h>

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
