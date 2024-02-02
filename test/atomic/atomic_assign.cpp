#include <core/atomics.h>

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