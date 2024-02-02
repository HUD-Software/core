#include <core/atomics.h>

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

GTEST_TEST(atomic, decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.decrement(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::relaxed), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::consume), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acquire), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::release), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, volatile_decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.decrement(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::relaxed), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::consume), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acquire), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::release), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.decrement(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, fetch_decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.fetch_decrement(), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, volatile_fetch_decrement_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.fetch_decrement(), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_decrement(hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, pre_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(--a, reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, volatile_pre_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(--a, reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, post_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a--, destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, volatile_post_decrement_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a--, destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, decrement_equal_operator_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a -= 1, reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}

GTEST_TEST(atomic, volatile_decrement_equal_operator_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a -= 1, reinterpret_cast<const type_t*>(destination_default_value - 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value - 1)); });
}