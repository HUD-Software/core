#include <core/atomics.h>

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
