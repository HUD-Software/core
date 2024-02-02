#include <core/atomics.h>

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

GTEST_TEST(atomic, increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.increment(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::relaxed), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::consume), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acquire), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::release), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, volatile_increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.increment(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::relaxed), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::consume), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acquire), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::release), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::acq_rel), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.increment(hud::atomics::memory_order_e::seq_cst), reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, fetch_increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.fetch_increment(), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, volatile_fetch_increment_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a.fetch_increment(), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::relaxed), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::consume), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acquire), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::release), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::acq_rel), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1));
        a = destination_default_value;

        hud_assert_eq(a.fetch_increment(hud::atomics::memory_order_e::seq_cst), destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, pre_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(++a, reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, volatile_pre_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(++a, reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, post_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a++, destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, volatile_post_increment_operator_pointer)
{

    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a++, destination_default_value);
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, increment_equal_operator_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a += 1, reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}

GTEST_TEST(atomic, volatile_increment_equal_operator_pointer)
{
    hud_test::for_each_type<i8, u8, i16, u16, i32, u32, i64, u64, usize, isize>()([]<typename type_t>()
                                                                                  {
        const type_t default_value{};
        const type_t*const destination_default_value = &default_value;
        volatile hud::atomic<const type_t*> a(destination_default_value);

        hud_assert_eq(a += 1, reinterpret_cast<const type_t*>(destination_default_value + 1));
        hud_assert_eq(a.load(), reinterpret_cast<const type_t*>(destination_default_value + 1)); });
}