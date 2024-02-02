#include <core/atomics.h>

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