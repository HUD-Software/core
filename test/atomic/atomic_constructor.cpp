#include <core/atomics.h>

GTEST_TEST(atomic, default_constructor_of_integral_do_not_initialize_atomic)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        alignas(sizeof(type_t)) u8 a[sizeof(hud::atomic<type_t>)];
        hud::memory::set(a, hud::u8_max);
        [[maybe_unused]] volatile unsigned char *p = (volatile unsigned char *)a;

        hud::atomic<type_t>* atomic = new (a) hud::atomic<type_t>;
        hud_assert_ne(atomic->load(), type_t{}); });
}

GTEST_TEST(atomic, default_constructor_of_pointer_do_not_initialize_atomic)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        alignas(type_t*) u8 a[sizeof(hud::atomic<type_t*>)];
        hud::memory::set(a, hud::u8_max);
        [[maybe_unused]] volatile unsigned char *p = (volatile unsigned char *)a;

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